//**-------------------------------------------------------**
//**
//**                     shader.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <d3d11.h>

class Shader
{
public:
	virtual bool Init() = 0;
	virtual bool CreateConstantBuffer() = 0;
	virtual bool InputLayout() = 0;
	bool CompileVS();
	bool CompilePS();
	bool CreateVS();
	bool CreatePS();
	void Uninit();
	ID3D11VertexShader*		GetVertexShader();
	ID3D11InputLayout*		GetInputLayout();
	ID3D11PixelShader*		GetPixelShader();
	ID3D11Buffer* const*	GetConstantBuffer();

protected:
	std::string				fileName_;
	ID3DBlob*				pCompiledVertexShader_ = nullptr;
	ID3DBlob*				pCompiledPixelShader_ = nullptr;
	ID3D11VertexShader*		pVertexShader_;
	ID3D11InputLayout*		pInputLayout_;
	ID3D11PixelShader*		pPixelShader_;
	ID3D11Buffer*			pConstantBuffer_;
};

#endif // !_SHADER_H_
