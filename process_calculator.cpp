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
	//高精度タイマー単位取得
	QueryPerformanceFrequency(&freq_);

	//待機時間計測用単位取得
	__int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	freqFrame_ = freq / 1000.0;
}

ProcessCalculator::~ProcessCalculator()
{

}

void ProcessCalculator::StartCalculate()
{
	//計測開始
	QueryPerformanceCounter(&calcStart_);
}

void ProcessCalculator::EndCalculate()
{
	//計測終了
	QueryPerformanceCounter(&calcEnd_);

	//処理時間算出(ms)
	processTime_ = (double)(calcEnd_.QuadPart - calcStart_.QuadPart) / freq_.QuadPart * 1000.0f;

	//最長処理時間計測開始カウンタ
	calcStartCounter_++;

	//一定フレーム数以降更新
	if (calcStartCounter_ >= CALC_START_COUNT)
	{
		calcStartCounter_ = CALC_START_COUNT;

		//最長処理時間更新(ms)
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