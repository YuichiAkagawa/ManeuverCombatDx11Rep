//**-------------------------------------------------------**
//**
//**                     texture.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <codecvt> 
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene_manager.h"
#include "WICTextureLoader.h"

#include "texture.h"

bool Texture::Init(int textureName)
{
	//�e�N�X�`���ǂݍ���
	ID3D11Resource *pResource = nullptr;

	//string��wstring�ɕϊ�
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wsc;
	std::wstring wsFileName = wsc.from_bytes(g_pTextureFile[textureName].fileName.c_str());

	HRESULT hr = DirectX::CreateWICTextureFromFile(Renderer::GetDevice(), wsFileName.c_str(), &pResource, &pTexture_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "ERROR", MB_OK);
		return false;
	}
	return true;
}

void Texture::Uninit()
{
	SafeRelease(pTexture_);
}

ID3D11ShaderResourceView* const* Texture::GetTexture()
{
	return &pTexture_;
}

bool TextureManager::Init()
{
	//NULL������
	for (int i = 0; i < TEXTURE_LIST::MAX; i++)
	{
		pInst_[i] = nullptr;
	}

	//�C���X�^���X����
	for (int i = 0; i < (int)SceneManager::GetScene()->GetSceneTexture().size(); i++)
	{
		pInst_[i] = new Texture;
		if (!pInst_[i]->Init(SceneManager::GetScene()->GetSceneTexture()[i]))
		{
			return false;
		}
	}
	return true;
}

void TextureManager::Uninit()
{
	//�C���X�^���X���
	for (int i = 0; i < (int)SceneManager::GetScene()->GetSceneTexture().size(); i++)
	{
		pInst_[i]->Uninit();
		SafeDelete(pInst_[i]);
	}

}

Texture* TextureManager::GetInstance(int textureName)
{
	int i = 0;

	for (i = 0; i < (int)SceneManager::GetScene()->GetSceneTexture().size(); i++)
	{
		if (SceneManager::GetScene()->GetSceneTexture()[i] == textureName && pInst_[i] != nullptr)
		{
			return pInst_[i];
			break;
		}
	}

	MessageBox(GetHWND(), "�e�N�X�`���̏����擾�ł��܂���ł���", "Error", MB_OK);
	return nullptr;
}