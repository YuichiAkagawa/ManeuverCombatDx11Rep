//**-------------------------------------------------------**
//**
//**                pixel_shader_test.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include "renderer.h"
#include "pixel_shader.h"
#include "pixel_shader_test.h"

static const std::string FILE_NAME = "test.hlsl";

bool PixelShaderTest::Init()
{
	fileName_ = FILE_NAME;

	return true;
}