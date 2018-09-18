//**-------------------------------------------------------**
//**
//**                vertex_shader_test.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _VERTEX_SHADER_TEST_
#define _VERTEX_SHADER_TEST_

class VertexShaderTest : public VertexShader
{
public:
	bool Init()override;
	bool InputLayout()override;
};

#endif // !_VERTEX_SHADER_TEST_
