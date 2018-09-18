//**-------------------------------------------------------**
//**
//**                     shader.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>

class Shader
{
public:
	virtual bool Init() = 0;
	virtual bool Compile();
	virtual bool Create();
	virtual bool InputLayout() = 0;
	virtual void Uninit();
	virtual ID3D11VertexShader* GetVertexShader();
	virtual ID3D11InputLayout* GetInputLayout();
	virtual ID3D11PixelShader* GetPixelShader();

protected:
	std::string				fileName_;
	ID3DBlob*				pCompiledVertexShader_ = nullptr;
	ID3DBlob*				pCompiledPixelShader_ = nullptr;
	ID3D11VertexShader*		pVertexShader_;
	ID3D11InputLayout*		pInputLayout_;
	ID3D11PixelShader*		pPixelShader_;
};

#endif // !_SHADER_H_
