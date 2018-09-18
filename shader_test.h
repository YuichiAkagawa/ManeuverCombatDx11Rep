//**-------------------------------------------------------**
//**
//**                   shader_test.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SHADER_TEST_H_
#define _SHADER_TEST_H_

class ShaderTest : public Shader
{
public:
	bool Init()override;
	bool InputLayout()override;
};

#endif // !_SHADER_TEST_H_
