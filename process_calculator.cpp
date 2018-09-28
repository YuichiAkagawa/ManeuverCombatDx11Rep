//**-------------------------------------------------------**
//**
//**               process_calculator.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <math.h>
#include "process_calculator.h"

constexpr DWORD CALC_START_COUNT = 60 * 2;

ProcessCalculator::ProcessCalculator() : processTime_(0.0), processTimeMax_(0.0), calcStartCounter_(0)
{
	//�����x�^�C�}�[�P�ʎ擾
	QueryPerformanceFrequency(&freq_);

	//�ҋ@���Ԍv���p�P�ʎ擾
	__int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	freqFrame_ = freq / 1000.0;
}

ProcessCalculator::~ProcessCalculator()
{

}

void ProcessCalculator::StartCalculate()
{
	//�v���J�n
	QueryPerformanceCounter(&calcStart_);
}

void ProcessCalculator::EndCalculate()
{
	//�v���I��
	QueryPerformanceCounter(&calcEnd_);

	//�������ԎZ�o(ms)
	processTime_ = (double)(calcEnd_.QuadPart - calcStart_.QuadPart) / freq_.QuadPart * 1000.0f;

	//�Œ��������Ԍv���J�n�J�E���^
	calcStartCounter_++;

	//���t���[�����ȍ~�X�V
	if (calcStartCounter_ >= CALC_START_COUNT)
	{
		calcStartCounter_ = CALC_START_COUNT;

		//�Œ��������ԍX�V(ms)
		processTimeMax_ = max(processTime_, processTimeMax_);
	}
}

double ProcessCalculator::GetProcessTimeMS()
{
	return processTime_;
}

double ProcessCalculator::GetProcessTimeMaxMS()
{
	return processTimeMax_;
}

void ProcessCalculator::Reset()
{
	processTime_ = 0.0;
	processTimeMax_ = 0.0;
	calcStartCounter_ = 0;
}