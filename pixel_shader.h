//**-------------------------------------------------------**
//**
//**                  pixel_shader.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _PIXEL_SHADER_H_
#define _PIXEL_SHADER_H_

#include <string>

class PixelShader
{
public:
	virtual bool Init() = 0;
	virtual bool Compile();
	virtual bool Create();
	virtual void Uninit();
	virtual ID3D11PixelShader* GetPixelShader();

protected:
	std::string				fileName_;
	ID3DBlob*				pCompiledShader_ = nullptr;
	ID3D11PixelShader*		pPixelShader_;
};

#endif // !_PIXEL_SHADER_H_
