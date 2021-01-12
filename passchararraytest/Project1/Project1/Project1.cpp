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

	SAFEARRAY*		pSafeArray = NULL;
	// Array of 3 SAFEARRAYBOUND structs. One for each dimension.
	SAFEARRAYBOUND	rgsabound[2];
	// Set the bounds information.
	// The following sets an array with the following dimensions :
	// [2][3][4].
	//
	// 24 elemenst in total. All elements laid out in
	// memory sequentially and contiguously.
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = 65;
	rgsabound[1].lLbound = 0;
	rgsabound[1].cElements = 20;
	// Create the SAFEARRAY.
	pSafeArray = SafeArrayCreate
	(
		VT_I1, // This signifies the integer type.
		2,  // This signifies 3 dimensions.
		rgsabound  // Bounds information.
	);


	//call the add method 
	pICalc->Add(55, 10, &IResult);
	pICalc->Passback(&pSafeArray);

	//move array to accesible place 
	// Get elements of the array and display the value.
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			long	rgIndices[2];
			int		value;
			// The indices of the array are specified
			// using "rgIndices".
			// Here the specificatio indicates that
			// we want to access the item at array
			// location : [i][j][k].
			rgIndices[0] = j;
			rgIndices[1] = i;
			SafeArrayGetElement
			(
				pSafeArray,
				rgIndices,
				(void FAR*)&value
			);
			IPassback[i][j] = value;
		}
	}

		//display the result 
	wprintf(L"The result is %d\n", IResult);

	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			std::cout << IPassback[i][j];
		}
		std::cout << std::endl;
	}

	//uninitialise com
	CoUninitialize();
	return 0;

}