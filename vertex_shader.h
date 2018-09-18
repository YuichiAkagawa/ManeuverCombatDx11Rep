//**-------------------------------------------------------**
//**
//**                  vertex_shader.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _VERTEX_SHADER_H_
#define _VERTEX_SHADER_H_

#include <string>

class VertexShader
{
public:
	virtual bool Init() = 0;
	virtual bool Compile();
	virtual bool Create();
	virtual bool InputLayout() = 0;
	virtual void Uninit();
	virtual ID3D11VertexShader* GetVertexShader();
	virtual ID3D11InputLayout* GetInputLayout();

protected:
	std::string				fileName_;
	ID3DBlob*				pCompiledShader_ = nullptr;
	ID3D11VertexShader*		pVertexShader_;
	ID3D11InputLayout*		pInputLayout_;
};

#endif // !_VERTEX_SHADER_H_
