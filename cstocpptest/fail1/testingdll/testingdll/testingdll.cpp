#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "tchar.h"

#import "C:\Users\deada\Desktop\C\cstocpptest\fail1\HelloDll\HelloDll\bin\Debug\HelloDll.tlb" raw_interfaces_only 

using namespace HelloDll;

int _tmain(int argc, _TCHAR* argv[])
{
	// initialise com
	HRESULT hr = CoInitialize(NULL);

	// create interface pointer 
	ICalculatorPtr pICalc(__uuidof(ManagedClass));

	long IResult = 0;

	// calling the add method
	// This looks weird. I'll come back to this //
	pICalc->Add(55, 10, &IResult);

	wprintf(L"the result is %d\n", IResult);

	// unintialistcoms
	CoUninitialize();

    return 0;
}
