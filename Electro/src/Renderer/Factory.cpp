//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#include "epch.hpp"
#include "Factory.hpp"
#include "Asset/AssetManager.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/EnvironmentMap.hpp"
#include "Platform/DX11/DX11VertexBuffer.hpp"
#include "Platform/DX11/DX11IndexBuffer.hpp"
#include "Platform/DX11/DX11Framebuffer.hpp"
#include "Platform/DX11/DX11Shader.hpp"
#include "Platform/DX11/DX11ConstantBuffer.hpp"
#include "Platform/DX11/DX11Pipeline.hpp"
#include "Platform/DX11/DX11Texture.hpp"

namespace Electro
{
    Ref<VertexBuffer> Factory::CreateVertexBuffer(Uint size)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11VertexBuffer>::Create(size);
        }
        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> Factory::CreateVertexBuffer(void* vertices, Uint size)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11VertexBuffer>::Create(vertices, size);
        }
        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> Factory::CreateIndexBuffer(void* indices, Uint count)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11IndexBuffer>::Create(indices, count);
        }
        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Framebuffer> Factory::CreateFramebuffer(const FramebufferSpecification& spec)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11: return Ref<DX11Framebuffer>::Create(spec);
        }
        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Shader> Factory::CreateShader(const String& filepath)
    {
        Ref<Shader> result;
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                result = AssetManager::Get<Shader>(filepath);
                if (!result)
                {
                    result = Ref<DX11Shader>::Create(filepath);
                    AssetManager::Submit<Shader>(result);
                }
        }

        return result;
    }

    Ref<ConstantBuffer> Factory::CreateConstantBuffer(Uint size, Uint bindSlot, DataUsage usage)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11ConstantBuffer>::Create(size, bindSlot, usage);
        }
        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Pipeline> Factory::CreatePipeline()
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11Pipeline>::Create();
        }
        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Factory::CreateTexture2D(Uint width, Uint height)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11Texture2D>::Create(width, height);
        }

        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Factory::CreateTexture2D(const String& path, bool srgb)
    {
        Ref<Texture2D> result = nullptr;
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                result = AssetManager::Get<Texture2D>(path);
                if (!result)
                {
                    result = Ref<DX11Texture2D>::Create(path, srgb);
                    AssetManager::Submit<Texture2D>(result);
                }
        }

        return result;
    }

    Ref<Cubemap> Factory::CreateCubemap(const String& path)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11Cubemap>::Create(path);
        }

        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<EnvironmentMap> Factory::CreateEnvironmentMap(const String& path)
    {
        Ref<EnvironmentMap> result = AssetManager::Get<EnvironmentMap>(path);
        if (!result)
        {
            result = Ref<EnvironmentMap>::Create(path);
            AssetManager::Submit<EnvironmentMap>(result);
        }
        return result;
    }

    Ref<Mesh> Factory::CreateMesh(const String& path)
    {
        return Ref<Mesh>::Create(path);
    }

    Ref<Material> Factory::CreateMaterial(const Ref<Shader>& shader, const String& nameInShader, const String& pathOrName)
    {
        Ref<Material> result = AssetManager::Get<Material>(pathOrName);
        if (!result)
        {
            result = Ref<Material>::Create(shader, nameInShader, pathOrName);
            AssetManager::Submit<Material>(result);
        }
        return result;
    }

    Ref<PhysicsMaterial> Factory::CreatePhysicsMaterial(const String& path)
    {
        Ref<PhysicsMaterial> result = AssetManager::Get<PhysicsMaterial>(path);
        if (!result)
        {
            result = Ref<PhysicsMaterial>::Create(path);
            AssetManager::Submit<PhysicsMaterial>(result);
        }
        return result;
    }

}
