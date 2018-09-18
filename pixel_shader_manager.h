//**-------------------------------------------------------**
//**
//**                pixel_shader_manager.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _PIXEL_SHADER_MANAGER_
#define _PIXEL_SHADER_MANAGER_

class PixelShader;

class PixelShaderManager
{
private:
	static bool Create();
public:
	enum
	{
		PS_UNKNOWN = -1,
		PS_TEST,
		PS_MAX
	};

	static bool Init();
	static void Uninit();
	static ID3D11PixelShader* GetPixelShader(int numPS);
private:
	static PixelShader* pPixelShader_[PS_MAX];

};

#endif // !_PIXEL_SHADER_MANAGER_
