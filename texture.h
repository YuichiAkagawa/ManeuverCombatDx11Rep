//**-------------------------------------------------------**
//**
//**                     texture.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <string>

struct TEXTURE_DATA
{
	std::string fileName;
	unsigned int width;
	unsigned int height;
};

static const TEXTURE_DATA g_pTextureFile[] =
{
	{ "resource/texture/sample.png", 256, 256 },
};

class Texture
{
private:
	ID3D11ShaderResourceView* pTexture_ = nullptr;
public:
	bool Init(int fileName);
	void Uninit();
	ID3D11ShaderResourceView* const* GetTexture();
};

class TextureManager
{
public:
	bool Init();
	void Uninit();
	Texture* GetInstance(int textureName);

	enum TEXTURE_LIST
	{
		UNKNOWN = -1,
		SAMPLE,
		MAX
	};

private:
	Texture* pInst_[TEXTURE_LIST::MAX];
};

#endif// !_TEXTURE_H_