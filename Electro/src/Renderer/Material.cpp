//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#include "epch.hpp"
#include "Material.hpp"
#include "Asset/AssetManager.hpp"

namespace Electro
{
    static Ref<Texture2D> sDummyTexture = nullptr;

    Material::Material(const Ref<Shader>& shader, const String& nameInShader, const String& name)
        : mName(name), mBufferName(nameInShader), mShader(shader)
    {
        mReflectionData = shader->GetReflectionData(ShaderDomain::Pixel);
        mTextures.resize(mReflectionData.GetResources().size());
        Allocate();
    }

    void Material::Allocate()
    {
        mCBufferMemory = Buffer();
        const ShaderBuffer& shaderBuffer = mReflectionData.GetBuffer(mBufferName);
        mCBufferMemory.Allocate(shaderBuffer.Size);
        mCBufferMemory.ZeroMem();
        mCBuffer = ConstantBuffer::Create(mCBufferMemory.GetSize(), shaderBuffer.Binding, DataUsage::DYNAMIC);
    }

    Material::~Material()
    {
        if (mCBufferMemory)
            mCBufferMemory.Release();
    }

    void Material::Bind() const
    {
        mShader->Bind();

        for (Uint i = 0; i < mTextures.size(); i++)
        {
            const Ref<Texture2D>& tex = mTextures[i];
            if (tex)
                mTextures[i]->PSBindAsShaderResource(i);
        }

        mCBuffer->PSBind();
        mCBuffer->SetDynamicData(mCBufferMemory.GetData());
    }

    Ref<Texture2D>& Material::GetTexture2D(const String& name)
    {
        for (const ShaderResource& res : mReflectionData.GetResources())
        {
            if (res.Name == name)
            {
                if (mTextures.size() > 1)
                    return mTextures[res.Binding];
            }
        }
        return sDummyTexture;
    }

    void Material::RemoveTexture2D(const String& name)
    {
        for (const ShaderResource& res : mReflectionData.GetResources())
        {
            if (res.Name == name)
            {
                Ref<Texture2D>& tex = mTextures[res.Binding];
                if(tex)
                    tex.Reset();
            }
        }
    }

    Ref<Material> Material::Create(const Ref<Shader>& shader, const String& nameInShader, const String& name)
    {
        return Ref<Material>::Create(shader, nameInShader, name);
    }
}
