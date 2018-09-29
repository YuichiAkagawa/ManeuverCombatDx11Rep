//**-------------------------------------------------------**
//**
//**                     model.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <codecvt> 
#include "load_fbx.h"
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene_manager.h"
#include "WICTextureLoader.h"
#include "model.h"

bool Model::Init(int modelName)
{
	pLoadFbx_ = new LoadFBX;

	if (!pLoadFbx_->Load(g_pModelFile[modelName], model_))
	{
		return false;
	}

	for (unsigned int i = 0; i < model_.mesh.size(); i++)
	{
		{
			//頂点バッファ作成
			D3D11_BUFFER_DESC vertexDesc;
			vertexDesc.ByteWidth = sizeof(VERTEX) * model_.mesh[i].vertex.size();
			vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexDesc.CPUAccessFlags = 0;
			vertexDesc.MiscFlags = 0;
			vertexDesc.StructureByteStride = 0;
			vertexDesc.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = &model_.mesh[i].vertex.front();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			HRESULT hr = Renderer::GetDevice()->CreateBuffer(
				&vertexDesc,
				&sd,
				&model_.mesh[i].vertexBuffer);
			if (FAILED(hr))
			{
				return false;
			}
		}

		{
			//インデックスバッファ作成
			D3D11_BUFFER_DESC indexDesc;
			indexDesc.ByteWidth = sizeof(WORD) * model_.mesh[i].index.size();
			indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexDesc.CPUAccessFlags = 0;
			indexDesc.MiscFlags = 0;
			indexDesc.StructureByteStride = 0;
			indexDesc.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = &model_.mesh[i].index.front();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			HRESULT hr = Renderer::GetDevice()->CreateBuffer(
				&indexDesc,
				&sd,
				&model_.mesh[i].indexBuffer);
			if (FAILED(hr))
			{
				return false;
			}
		}

		{
			//テクスチャ読み込み
			ID3D11Resource *pResource = nullptr;

			//stringをwstringに変換
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wsc;
			std::wstring wsFileName = wsc.from_bytes(model_.mesh[i].material.textureName.c_str());

			HRESULT hr = DirectX::CreateWICTextureFromFile(Renderer::GetDevice(), wsFileName.c_str(), &pResource, &model_.mesh[i].material.texture);
			if (FAILED(hr))
			{
				//stringをwstringに変換
				std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wscWhite;
				std::wstring wsFileNameWhite = wscWhite.from_bytes("resource/model/white.png");

				hr = DirectX::CreateWICTextureFromFile(Renderer::GetDevice(), wsFileNameWhite.c_str(), &pResource, &model_.mesh[i].material.texture);

				if (FAILED(hr))
				{
					MessageBox(GetHWND(), "テクスチャの読み込みに失敗しました", "ERROR", MB_OK);
					return false;
				}
			}
		}
	}
	
	return true;
}

void Model::Uninit()
{
	delete pLoadFbx_;
}

void Model::Draw()
{
	for (unsigned int i = 0; i < model_.mesh.size(); i++)
	{
		//頂点バッファセット
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &model_.mesh[i].vertexBuffer, &stride, &offset);

		//インデックスバッファセット
		Renderer::GetDeviceContext()->IASetIndexBuffer(model_.mesh[i].indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		//描画方法
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//カラーマップセット
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &model_.mesh[i].material.texture);

		//描画
		Renderer::GetDeviceContext()->DrawIndexed(model_.mesh[i].index.size(), 0, 0);
	}
}

bool ModelManager::Init()
{
	//NULL初期化
	for (int i = 0; i < MODEL_LIST::MAX; i++)
	{
		pInst_[i] = nullptr;
	}

	//インスタンス生成
	for (int i = 0; i < (int)SceneManager::GetScene()->GetSceneModel().size(); i++)
	{
		pInst_[i] = new Model;
		if (!pInst_[i]->Init(SceneManager::GetScene()->GetSceneModel()[i]))
		{
			return false;
		}
	}
	return true;
}

void ModelManager::Uninit()
{
	//インスタンス解放
	for (int i = 0; i < (int)SceneManager::GetScene()->GetSceneModel().size(); i++)
	{
		pInst_[i]->Uninit();
		SafeDelete(pInst_[i]);
	}
}

Model* ModelManager::GetInstance(int modelName)
{
	int i = 0;

	for (i = 0; i < (int)SceneManager::GetScene()->GetSceneModel().size(); i++)
	{
		if (SceneManager::GetScene()->GetSceneModel()[i] == modelName && pInst_[i] != nullptr)
		{
			return pInst_[i];
			break;
		}
	}

	MessageBox(GetHWND(), "モデルの情報を取得できませんでした", "Error", MB_OK);
	return nullptr;
}