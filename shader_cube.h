//**-------------------------------------------------------**
//**
//**                   shader_cube.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SHADER_CUBE_H__
#define _SHADER_CUBE_H__

class ShaderCube : public Shader
{
public:
	bool Init()override;
	bool InputLayout()override;
};

#endif // !_SHADER_CUBE_H_
