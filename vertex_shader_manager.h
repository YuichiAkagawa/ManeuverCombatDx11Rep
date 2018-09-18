//**-------------------------------------------------------**
//**
//**                vertex_shader_manager.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _VERTEX_SHADER_MANAGER_
#define _VERTEX_SHADER_MANAGER_

class VertexShader;

class VertexShaderManager
{
private:
	static bool Create();
public:
	enum
	{
		VS_UNKNOWN = -1,
		VS_TEST,
		VS_MAX
	};

	static bool Init();
	static void Uninit();
	static ID3D11VertexShader* GetVertexShader(int numVS);
	static ID3D11InputLayout* GetInputLayout(int numVS);
private:
	static VertexShader* pVertexShader_[VS_MAX];

};

#endif // !_VERTEX_SHADER_MANAGER_
