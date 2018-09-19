//**-------------------------------------------------------**
//**
//**                  actor_manager.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_MANAGER_H_
#define _ACTOR_MANAGER_H_

#include <vector>
#include <algorithm>
#include <typeinfo>

class Actor;
class TextureManager;

class ActorManager
{
private:
	std::vector<Actor*> actorList_;
	TextureManager* pTextureManager_;

	void Stats();
	void CheckType(const type_info* typeInfo);
public:
	ActorManager(){}
	~ActorManager(){}
	bool Init();
	void Uninit();
	void Update();
	void Draw();
	Actor* GetActor(int name, int number);
	void SetTextureManager(TextureManager* pTextureManager);

	template <typename T>
	int CreateActor(T* pActor)
	{
		int number = 0;
		std::for_each(actorList_.begin(), actorList_.end(), [&](Actor* pAllActor)
		{
			//�����A�N�^���Z�o �A�N�^�ԍ�����
			if (pAllActor->GetName() == pActor->GetName())
			{
				number++;
			}
		});
		pActor->SetNumber(number);

		//�e�N�X�`���}�l�[�W���̃|�C���^�Z�b�g
		pActor->SetTextureManager(pTextureManager_);

		//�A�N�^�̌^���Z�b�g
		pActor->SetTypeInfo(&typeid(pActor));

		//���X�g�Ɋi�[
		actorList_.emplace_back(pActor);

		return number;
	}

	enum NAME
	{
		NAME_UNKNOWN = 0,
		NAME_CAMERA,
		NAME_FREE_CAMERA,
		NAME_CAMERA_SELECTER,
		NAME_MAX
	};

	enum TAG
	{
		TAG_UNKNOWN = 0,
		TAG_CAMERA,
		TAG_CAMERA_SELECTER,
		TAG_MAX
	};

	enum LAYER
	{
		LAYER_UNKNOWN = 0,
		LAYER_ETC,
		LAYER_MAX
	};
};

#endif // !_ACTOR_MANAGER_H_
