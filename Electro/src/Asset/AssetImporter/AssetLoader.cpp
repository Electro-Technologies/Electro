//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#pragma once
#include "AssetLoader.hpp"
#include "Asset/AssetManager.hpp"
#include "TextureLoader.hpp"

namespace Electro
{
    std::unordered_map<AssetType, Scope<IAssetLoader>> AssetLoader::sLoaders;

    void AssetLoader::Init()
    {
        sLoaders[AssetType::TEXTURE2D] = CreateScope<TextureLoader>();
        sLoaders[AssetType::ENVIRONMENT_MAP] = CreateScope<EnvMapLoader>();
    }

    void AssetLoader::Shutdown()
    {
        sLoaders.clear();
    }

    bool AssetLoader::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
    {
        if (sLoaders.find(metadata.Type) == sLoaders.end())
        {
            Log::Warn("There is currently no loaders for assets of type {0}", metadata.Path.extension().string());
            return false;
        }

        return sLoaders[metadata.Type]->LoadData(metadata, asset);
    }

    bool AssetLoader::Serialize(const AssetMetadata& metadata, Ref<Asset>& asset)
    {
        if (sLoaders.find(metadata.Type) == sLoaders.end())
        {
            Log::Warn("There is currently no serializer for assets of type {0}", metadata.Path.extension().string());
            return false;
        }

        return sLoaders[metadata.Type]->SaveData(metadata, asset);
    }
}