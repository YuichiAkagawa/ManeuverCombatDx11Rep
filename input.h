//**-------------------------------------------------------**
//**
//**                    input.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _INPUT_H_
#define _INPUT_H_

#include <dinput.h>

HRESULT		InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void		UninitKeyboard();
void		UpdateKeyboard();

HRESULT		InitInput(HINSTANCE hInstance, HWND hWnd);
void		UninitInput();
void		UpdateInput();

bool		GetKeyboardPress(int nKey);
bool		GetKeyboardTrigger(int nKey);
bool		GetKeyboardRepeat(int nKey);
bool		GetKeyboardRelease(int nKey);

#endif// !_INPUT_H_