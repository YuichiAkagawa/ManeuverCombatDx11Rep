//**-------------------------------------------------------**
//**
//**                    input.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "input.h"

constexpr int NUM_KEY_MAX = 256;			//�L�[�̍ő吔
constexpr int LIMIT_COUNT_REPEAT = 20;		//���s�[�g�J�E���g���~�b�^�[

LPDIRECTINPUT8			g_pInput = nullptr;					//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = nullptr;			//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
BYTE					g_aKeyState[NUM_KEY_MAX];			//�L�[�{�[�h�̓��͏�񃏁[�N
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	//�L�[�{�[�h�̃g���K�[��񃏁[�N
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	//�L�[�{�[�h�̃����[�X��񃏁[�N
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		//�L�[�{�[�h�̃��s�[�g��񃏁[�N
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	//�L�[�{�[�h�̃��s�[�g�J�E���^

//���͏����̏�����
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);

	if (g_pInput == nullptr)
	{
		//DirectInput�I�u�W�F�N�g�̍쐬
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pInput, nullptr)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//���͏����̏I������
void UninitInput()
{
	if (g_pInput != nullptr)
	{
		//DirectInput�I�u�W�F�N�g�̊J��
		g_pInput->Release();
		g_pInput = nullptr;
	}
}

//���͏����̍X�V����
void UpdateInput()
{
}

//�L�[�{�[�h�̏�����
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//���͏����̏�����
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����܂���ł����B", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	//�f�o�C�X�̍쐬
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, nullptr)))
	{
		MessageBox(hWnd, "�L�[�{�[�h������܂���ł����B", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	//�������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND/*DISCL_BACKGROUND*/ | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//�L�[�{�[�h�̏I������
void UninitKeyboard()
{
	if (g_pDevKeyboard != nullptr)
	{//���̓f�o�C�X(�L�[�{�[�h)�̊J��
	 //�L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = nullptr;
	}

	//���͏����̏I������
	UninitInput();
}

//�L�[�{�[�h�̍X�V����
void UpdateKeyboard()
{
	BYTE aKeyState[NUM_KEY_MAX];

	//�f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			//�L�[�g���K�[�E�����[�X���𐶐�
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & ~aKeyState[nCnKey];

			//�L�[���s�[�g���𐶐�
			if (aKeyState[nCnKey])
			{
				if (g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if (g_aKeyStateRepeatCnt[nCnKey] == 1
						|| g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{//�L�[�������n�߂��ŏ��̃t���[���A�܂��͈�莞�Ԍo�߂�����L�[���s�[�g���ON
						g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
					}
					else
					{
						g_aKeyStateRepeat[nCnKey] = 0;
					}
				}
			}
			else
			{
				g_aKeyStateRepeatCnt[nCnKey] = 0;
				g_aKeyStateRepeat[nCnKey] = 0;
			}

			//�L�[�v���X����ۑ�
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}
}

//�L�[�{�[�h�̃v���X��Ԃ��擾
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h�̃g���K�[��Ԃ��擾
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h�̃��s�[�g��Ԃ��擾
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h�̃����|�X��Ԃ��擾
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
