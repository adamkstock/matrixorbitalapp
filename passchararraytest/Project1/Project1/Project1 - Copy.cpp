#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "tchar.h"

#import "C:\Users\deada\Desktop\C\passchararraytest\ClassLibrary1\ClassLibrary1\bin\Debug\ClassLibrary1.tlb" raw_interfaces_only

using namespace ClassLibrary1;

int _tmain(int argc, _TCHAR* argv[])
{
	//initialise com
	HRESULT hr = CoInitialize(NULL);


	//create interface pointer 
	ICalculatorPtr pICalc(__uuidof(ManagedClass));

	//placwe to store result 
	long IResult = 0;
	char IPassback [65][20];


	//call the add method 
	pICalc->Add(55, 10, &IResult);


	//IPassback = (pICalc->Passback);
	//for (int i = 1; i < 65; i++)
	//{
	//	for (int j = 0; j < 19; j++)
	//	{
	//		pICalc->Passback[i][j](&IPassback[i][j]);
	//	}
	//}
	//pICalc->Passback[][](&IPassback[][]);

	//display the result 
	wprintf(L"The result is %d\n", IResult);

	//uninitialise com
	CoUninitialize();
	return 0;

}