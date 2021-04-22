//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#pragma once
#include "Renderer/ElectroTexture.hpp"
#include <d3d11.h>

namespace Electro
{
    class DX11Texture2D : public Texture2D
    {
    public:
        DX11Texture2D(Uint width, Uint height);
        DX11Texture2D(const String& path, bool srgb = false, bool flipped = false);
        ~DX11Texture2D();
        virtual void Bind(Uint bindslot = 0, ShaderDomain domain = ShaderDomain::PIXEL) const override;
        virtual const String GetName() const override { return mName; }
        virtual Uint GetWidth()  const override { return mWidth; }
        virtual Uint GetHeight() const override { return mHeight; }
        virtual String GetFilepath() const override { return mFilepath; }
        virtual RendererID GetRendererID() const override { return (RendererID)mSRV; }
        virtual void SetData(void* data, Uint size) override;
        virtual bool Loaded() override { return mLoaded; };
        virtual void ReloadFlipped() override;
        virtual bool& GetFlipStatus() override { return mIsFlipped; }
        virtual void Unbind() const override {}
        virtual bool operator ==(const Texture2D& other) const override { return mSRV == ((DX11Texture2D&)other).mSRV; }
    private:
        void LoadTexture(bool flip);
    private:
        ID3D11Texture2D* mTexture2D;
        ID3D11ShaderResourceView* mSRV;

        Uint mWidth, mHeight;
        String mFilepath;
        String mName;
        bool mSRGB;
        bool mIsFlipped;
        bool mIsHDR = false;
        bool mLoaded = false;
    };

    class DX11TextureCube : public TextureCube
    {
    public:
        DX11TextureCube(const String& path);
        ~DX11TextureCube();
        virtual void Bind(Uint slot = 0, ShaderDomain domain = ShaderDomain::PIXEL) const override;
        virtual RendererID GenIrradianceMap() override;
        virtual RendererID GenPreFilter() override;
        virtual void BindIrradianceMap(Uint slot) override;
        virtual void BindPreFilterMap(Uint slot) override;
        virtual String GetPath() const override { return mPath; }
        virtual String const GetName() const override { return mName; }
        virtual RendererID GetRendererID() const override { return (RendererID)mSRV; }

        virtual bool operator ==(const TextureCube& other) const override { return mSRV == ((DX11TextureCube&)other).mSRV; }
    private:
        void LoadTextureCube();
    private:
        String mPath;
        String mName;
        ID3D11ShaderResourceView* mSRV = nullptr;
        ID3D11ShaderResourceView* mIrradianceSRV = nullptr;
        ID3D11ShaderResourceView* mPreFilterSRV = nullptr;
    };
}
