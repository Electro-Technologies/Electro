//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#include "epch.hpp"
#include "Texture.hpp"
#include "Asset/AssetManager.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Platform/DX11/DX11Texture.hpp"

namespace Electro
{
    Ref<Texture2D> Texture2D::Create(Uint width, Uint height)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11Texture2D>::Create(width, height);
        }
        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const String& path, bool srgb)
    {
        Ref<Texture2D> result = nullptr;
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                result = AssetManager::Get<Texture2D>(AssetManager::GetHandle(path));
                if (!result)
                {
                    result = Ref<DX11Texture2D>::Create(path, srgb);
                    AssetManager::Submit<Texture2D>(result);
                }
        }
        return result;
    }

    Ref<Cubemap> Cubemap::Create(const String& path)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::DX11:
                return Ref<DX11Cubemap>::Create(path);
        }

        E_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }
}