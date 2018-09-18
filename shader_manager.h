//**-------------------------------------------------------**
//**
//**                shader_manager.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SHADER_MANAGER_
#define _SHADER_MANAGER_

class VertexShader;
class PixelShader;

class ShaderManager
{
private:
	static bool Create();
public:
	enum
	{
		UNKNOWN = -1,
		TEST,
		MAX
	};

	static bool Init();
	static void Uninit();
	static ID3D11VertexShader* GetVertexShader(int num);
	static ID3D11InputLayout* GetInputLayout(int num);
	static ID3D11PixelShader* GetPixelShader(int num);
private:
	static VertexShader* pVertexShader_[MAX];
	static PixelShader* pPixelShader_[MAX];
};

#endif // !_SHADER_MANAGER_
