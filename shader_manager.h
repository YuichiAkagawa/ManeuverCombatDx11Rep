//**-------------------------------------------------------**
//**
//**                shader_manager.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SHADER_MANAGER_
#define _SHADER_MANAGER_

class Shader;

class ShaderManager
{
private:
	static bool Create();
public:
	enum
	{
		UNKNOWN = -1,
		TEST,
		CUBE,
		MAX
	};

	static bool Init();
	static void Uninit();
	static ID3D11VertexShader* GetVertexShader(int num);
	static ID3D11InputLayout* GetInputLayout(int num);
	static ID3D11PixelShader* GetPixelShader(int num);
	static ID3D11Buffer* const* GetConstantBuffer(int num);
private:
	static Shader* pShader_[MAX];
};

#endif // !_SHADER_MANAGER_
