//**-------------------------------------------------------**
//**
//**               process_calculator.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _PROCESS_CALCULATOR_H_
#define _PROCESS_CALCULATOR_H_

#include <Windows.h>

class ProcessCalculator
{
private:
	LARGE_INTEGER				freq_;
	double						freqFrame_;

	LARGE_INTEGER				calcStart_;
	LARGE_INTEGER				calcEnd_;
	double						processTime_;
	double						processTimeMax_;
	DWORD						calcSpanCounter_;
	DWORD						calcStartCounter_;

public:
	ProcessCalculator();
	~ProcessCalculator();
	void StartCalculate();
	void EndCalculate();
	double GetProcessTimeMS();
	double GetProcessTimeMaxMS();
	void Reset();
};

#endif // !_PROCESS_CALCULATOR_H_
