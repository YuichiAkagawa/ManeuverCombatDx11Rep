//**-------------------------------------------------------**
//**
//**                  actor_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <typeinfo>
#include "imgui/imgui.h"
#include "actor.h"
#include "main.h"
#include "process_calculator.h"
#include "texture.h"
#include "actor_manager.h"

bool ActorManager::Init()
{
	bool error = true;

	//�C���X�^���X������
	std::for_each(actorList_.begin(), actorList_.end(), [&](Actor* pAllActor)
	{
		if (!pAllActor->Init())
		{
			error = false;
		}
	});

	return error;
}

void ActorManager::Uninit()
{
	//�C���X�^���X�j��
	std::for_each(actorList_.begin(), actorList_.end(), [&](Actor* pAllActor)
	{
		pAllActor->Uninit();
		SafeDelete(pAllActor);
	});
}

void ActorManager::Update()
{
	for (int name = NAME_UNKNOWN; name < NAME_MAX; name++)
	{
		std::for_each(actorList_.begin(), actorList_.end(), [&](Actor* pAllActor)
		{
			//�A�N�^����v�@�X�V�L��
			if (pAllActor->GetName() == name && pAllActor->GetIsUpdate())
			{
				//�X�V����
				pAllActor->GetUpdateProcessCalc().StartCalculate();
				pAllActor->Update();
				pAllActor->GetUpdateProcessCalc().EndCalculate();
			}
		});
	}

	//Stats�\��
	Stats();
}

void ActorManager::Draw()
{
	for (int layer = LAYER_MAX; layer >= LAYER_UNKNOWN; layer--)
	{
		std::for_each(actorList_.begin(), actorList_.end(), [&](Actor* pAllActor)
		{
			//�A�N�^����v�@�`��L��
			if (pAllActor->GetLayer() == layer && pAllActor->GetIsDraw())
			{
				//�`�揈��
				pAllActor->GetDrawProcessCalc().StartCalculate();
				pAllActor->Draw();
				pAllActor->GetDrawProcessCalc().EndCalculate();
			}
		});
	}
}

Actor* ActorManager::GetActor(int name, int number)
{
	auto itr = std::find_if(actorList_.begin(), actorList_.end(), [&](Actor* pAllActor)
	{
		return
			//���O�Ɣԍ�����v����A�N�^��T��
			pAllActor->GetName() == name && pAllActor->GetNumber() == number;
	});

	if (itr != actorList_.end())
	{
		return (*itr);
	}
	else
	{
		//�Y������|�C���^���Ȃ������ꍇnull
		return nullptr;
	}
}

void ActorManager::SetTextureManager(TextureManager* pTextureManager)
{
	pTextureManager_ = pTextureManager;
}

void ActorManager::Stats()
{
	//Actor���X�g�̕\��
	ImGui::Begin("Stats");
	if (ImGui::BeginMenu("Actor"))
	{

		CheckType(&typeid(Actor*));
		ImGui::EndMenu();
	}
	ImGui::End();

	//Stats�\��
	for (int name = NAME_UNKNOWN; name < NAME_MAX; name++)
	{
		std::for_each(actorList_.begin(), actorList_.end(), [&](Actor* pAllActor)
		{
			if (pAllActor->GetName() == name)
			{
				pAllActor->Stats();
			}
		});
	}
}

void ActorManager::CheckType(const type_info* typeInfo)
{
	std::for_each(actorList_.begin(), actorList_.end(), [&](Actor* pAllActor)
	{
		if (*pAllActor->GetTypeInfo() == *typeInfo)
		{
			if (ImGui::MenuItem(pAllActor->GetNameUnique().c_str()))
			{
				pAllActor->SetIsDrawImgui(true);
			}
		}
	});
}