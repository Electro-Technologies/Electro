//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#include "epch.hpp"
#include "ElectroShaderCompiler.hpp"
#include "Core/System/ElectroOS.hpp"
#include "ElectroReflectionData.hpp"
#include <SPIRV-Cross/spirv.hpp>
#include <SPIRV-Cross/spirv_glsl.hpp>
#include <SPIRV-Cross/spirv_hlsl.hpp>
#include "Renderer/Interface/ElectroVertexBuffer.hpp"

namespace Electro
{
    namespace Utils
    {
        ShaderDataType SPIRvCrossTypeToElectroType(const spirv_cross::SPIRType& type, const Uint width)
        {
            switch (type.basetype)
            {
            case spirv_cross::SPIRType::Int:
            {
                switch (width)
                {
                    case 1: return ShaderDataType::Int;
                    case 2: return ShaderDataType::Int2;
                    case 3: return ShaderDataType::Int3;
                    case 4: return ShaderDataType::Int4;
                }
            }
            case spirv_cross::SPIRType::Float:
            {
                switch (width)
                {
                    case  1: return ShaderDataType::Float;
                    case  2: return ShaderDataType::Float2;
                    case  3: return ShaderDataType::Float3;
                    case  4: return ShaderDataType::Float4;
                    case  9: return ShaderDataType::Mat3;
                    case 16: return ShaderDataType::Mat4;
                }
            }
            case spirv_cross::SPIRType::Boolean:
            {
                return ShaderDataType::Bool;
            }
            }
        }
    }

    SPIRVHandle ShaderCompiler::CompileToSPIRv(const String& name, const String& shaderSource, const ShaderDomain& domain, const bool removeOld)
    {
        Vector<Uint> result;
        String extension = "";
        String stage = "";

        switch (domain)
        {
            case ShaderDomain::VERTEX:
                extension = "vert.hlsl";
                stage     = "vertex"; break;

            case ShaderDomain::PIXEL:
                extension = "pixel.hlsl";
                stage     = "fragment"; break;

            case ShaderDomain::COMPUTE:
                extension = "compute.hlsl";
                stage     = "compute"; break;
        }

        String filepath = "Electro/assets/SPIRvCache/" + OS::GetNameWithoutExtension(name) + extension;
        String spvFilePath = filepath + ".spv";

        // Make sure we have the required folder
        OS::CreateOrEnsureFolderExists("Electro/assets/SPIRvCache");

        if (!removeOld)
        {
            if (OS::FileExists(spvFilePath.c_str()))
            {
                std::ifstream in(spvFilePath, std::ios::in | std::ios::binary);
                if (in.is_open())
                {
                    in.seekg(0, std::ios::end);
                    auto size = in.tellg();
                    in.seekg(0, std::ios::beg);

                    result.resize(size / sizeof(Uint));
                    in.read((char*)result.data(), size);
                    in.close();
                }
                else
                    ELECTRO_ERROR("Cannot open filepath %s", spvFilePath.c_str());

                return SPIRVHandle(result, OS::GetNameWithExtension(spvFilePath.c_str()), domain);
            }
        }

        //Generate SPIR-V(.spv)
        std::ofstream out(filepath, std::ios::out);
        if (out)
        {
            out.write(shaderSource.c_str(), shaderSource.size());
            out.flush();
            out.close();
        }
        else
            ELECTRO_ERROR("Cannot open filepath %s", filepath.c_str());

        String command = "glslc.exe -fshader-stage=" + stage + " -c " + filepath + " -o " + spvFilePath;
        OS::RunInTerminal(command.c_str());

        std::ifstream in(spvFilePath, std::ios::in | std::ios::binary);
        if (in.is_open())
        {
            in.seekg(0, std::ios::end);
            auto size = in.tellg();
            in.seekg(0, std::ios::beg);

            result.resize(size / sizeof(Uint));
            in.read((char*)result.data(), size);
            in.close();
        }
        else
            ELECTRO_ERROR("Cannot open filepath %s", spvFilePath.c_str());

        return SPIRVHandle(result, OS::GetNameWithExtension(spvFilePath.c_str()), domain);
    }

    static void PrepareCompilation(spirv_cross::Compiler& compiler)
    {
        Uint sampler = compiler.build_dummy_sampler_for_combined_images();
        if (sampler != 0)
        {
            compiler.set_decoration(sampler, spv::DecorationDescriptorSet, 0);
            compiler.set_decoration(sampler, spv::DecorationBinding, 0);
        }

        compiler.build_combined_image_samplers();
        for (auto& remap : compiler.get_combined_image_samplers())
            compiler.set_name(remap.combined_id, spirv_cross::join("Electro_SPIRV_Cross_Combined_", compiler.get_name(remap.image_id), compiler.get_name(remap.sampler_id)));
    }

    ShaderReflectionData ShaderCompiler::Reflect(const SPIRVHandle& spirv, const String& shaderName)
    {
        spirv_cross::Compiler compiler(spirv.SPIRV);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        ShaderReflectionData result;
        result.SetDomain(spirv.Domain);

        for (const spirv_cross::Resource& resource : resources.separate_images)
        {
            ShaderResource res;
            res.Binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            res.Name = resource.name.c_str();
            result.PushResource(res);
        }

        for (const spirv_cross::Resource& resource : resources.uniform_buffers)
        {
            ShaderBuffer buffer;
            const spirv_cross::SPIRType& bufferType = compiler.get_type(resource.base_type_id);

            buffer.Size = compiler.get_declared_struct_size(bufferType);
            buffer.Binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            buffer.BufferName = resource.name.c_str();

            for (Uint i = 0; i < bufferType.member_types.size(); i++)
            {
                const spirv_cross::SPIRType& temp = compiler.get_type(bufferType.member_types[i]);

                ShaderBufferMember bufferMember;
                bufferMember.Name = buffer.BufferName + '.' +  compiler.get_member_name(bufferType.self, i);
                bufferMember.Type = Utils::SPIRvCrossTypeToElectroType(temp, temp.vecsize * temp.columns);
                bufferMember.MemoryOffset = compiler.type_struct_member_offset(bufferType, i); //In bytes
                buffer.Members.emplace_back(bufferMember);
            }

            result.ValidateBuffer(buffer);
            result.PushBuffer(buffer);
        }

        return result;
    }

    String ShaderCompiler::CrossCompileToGLSL(const SPIRVHandle& spirv)
    {
        E_ASSERT(spirv.IsValid(), "Invalid SPIR-V handle!");

        spirv_cross::CompilerGLSL compiler(spirv.SPIRV);
        PrepareCompilation(compiler);
        String result = compiler.compile();
        //std::cout << result << std::endl; //Uncomment to print output
        //std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        return result;
    }

    String ShaderCompiler::CrossCompileToHLSL(const SPIRVHandle& spirv)
    {
        E_ASSERT(spirv.IsValid(), "Invalid SPIR-V handle!");

        spirv_cross::CompilerHLSL compiler(spirv.SPIRV);
        PrepareCompilation(compiler);

        spirv_cross::CompilerHLSL::Options options;

        //Set the shader model to 5.0
        options.shader_model = 50;
        compiler.set_hlsl_options(options);

        String result = compiler.compile();
        //std::cout << result << std::endl; //Uncomment to print output
        //std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        return result;
    }
}
