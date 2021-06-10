//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#pragma once
#include "Core/Ref.hpp"
#include "Asset/AssetBase.hpp"
#include "Renderer/Interface/Shader.hpp"
#include <glm/glm.hpp>

namespace Electro
{
    class Texture2D : public Asset
    {
    public:
        virtual ~Texture2D() = default;

        //Returns Width of the loaded Texture2D
        virtual Uint GetWidth() const = 0;

        //Returns Height of the loaded Texture2D
        virtual Uint GetHeight() const = 0;

        //Returns the RendererID, used for the texture
        virtual RendererID GetRendererID() const = 0;

        //You can manually set the data for the texture by this function, useful for dynamic/custom textures
        virtual void SetData(void* data, Uint size) = 0;

        //Returns true if the texture is loaded to Electro perfectly
        virtual bool Loaded() = 0;

        //Binds the Texture2D to the pipeline
        virtual void VSBind(Uint slot = 0) const = 0;
        virtual void PSBind(Uint slot = 0) const = 0;
        virtual void CSBind(Uint slot = 0) const = 0;

        //Unbinds the Texture2D from the pipeline [Only works for pixel shader]
        virtual void Unbind(Uint slot) const = 0;

        //Calculates the MipMap count
        virtual Uint CalculateMipMapCount(Uint width, Uint height) = 0;

        virtual bool operator==(const Texture2D& other) const = 0;

        static Ref<Texture2D> Create(Uint width, Uint height);
        static Ref<Texture2D> Create(const String& path, bool srgb = false);
    };

    class Cubemap : public IElectroRef
    {
    public:
        virtual ~Cubemap() = default;

        //Returns the RendererID, used for the texture
        virtual RendererID GetRendererID() const = 0;

        //Returns the Path of the HDR texture
        virtual String GetPath() const = 0;

        //Returns the file name
        virtual String const GetName() const = 0;

        //Binds the Cubemap to the pipeline
        virtual void VSBind(Uint slot = 0) const = 0;
        virtual void PSBind(Uint slot = 0) const = 0;
        virtual void CSBind(Uint slot = 0) const = 0;

        //Binds the Cubemap from the pipeline
        virtual void Unbind(Uint slot = 0, ShaderDomain domain = ShaderDomain::Pixel) const = 0;

        //Generates the PreFilter map for the texture cube
        virtual RendererID GenIrradianceMap() = 0;

        //Generates the irradince map for the texture cube
        virtual RendererID GenPreFilter() = 0;

        //Binds the irradince map which was generated for this texture cube, remember to generate one before calling this via GenIrradianceMap();
        virtual void BindIrradianceMap(Uint slot) const = 0;

        //Binds the PreFilter map which was generated for this texture cube, remember to generate one before calling this via GenPreFilter();
        virtual void BindPreFilterMap(Uint slot) const = 0;

        virtual bool operator==(const Cubemap& other) const = 0;

        //Calculates the MipMap count
        virtual Uint CalculateMipMapCount(Uint width, Uint height) = 0;

        static Ref<Cubemap> Create(const String& path);
    };
}