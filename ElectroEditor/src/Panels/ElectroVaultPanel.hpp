//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#pragma once
#include "Core/ElectroBase.hpp"
#include "Renderer/Interface/ElectroTexture.hpp"
#include <glm/glm.hpp>

namespace Electro
{
    struct DirectoryEntry
    {
        String Name;
        String Extension;
        String AbsolutePath;
        String ParentFolder;
        bool IsDirectory;
    };

    Ref<Texture2D>& GetTexturePreviewtorage();
    class VaultPanel
    {
    public:
        VaultPanel(const void* editorLayerPtr);
        ~VaultPanel() = default;

        void Init();
        void OnImGuiRender(bool* show);
    private:
        void DrawPath(DirectoryEntry& entry);
        Vector<DirectoryEntry> GetFiles(const String& directory);
        void DrawImageAtMiddle(const glm::vec2& imageRes, const glm::vec2& windowRes);
    private:
        Vector<DirectoryEntry> mFiles;
        String mProjectPath;
        String mDrawingPath;
        RendererID mFolderTextureID;
        RendererID mCSTextureID;
        RendererID m3DFileTextureID;
        RendererID mElectroTextureID;
        RendererID mUnknownTextureID;
    };
}
