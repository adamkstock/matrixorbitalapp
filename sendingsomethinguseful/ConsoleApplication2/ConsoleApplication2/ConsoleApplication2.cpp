#include <iostream>
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <string>

#using <System.dll>

#define SystemProcessorPerformanceInformation 0x8
#define SystemBasicInformation    0x0

using namespace std;
using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;

string movetooleft(string line)								
{
	string data = line;
	int length = data.length();

	for (int i = 1; i < length; i++)																	//loop to move each value one position to the left 
	{
		data[i] = data[(i + 1)];
	}
	data[(length - 1)] = ' ';																			//add a spare space to keep length the same 

	return data;
}

void makelengthfifty(string messages[])
{
	//make all strings 50 chars long
	for (int i = 0; i < 128; i++)						//loop for every message 
	{
		while (messages[i].length() < 49)				
		{
			messages[i] = messages[i] + " ";			//add spaces to the end of messages till it is 49 chars long 
		}
		messages[i] = messages[i] + '\0';				//add terminator char 
	}
}

int sendtooscreen(string messages[], int screenlength, int screenheight, int scrolldelay)
{
	//setup the com port 
	SerialPort^ mySerialPort = gcnew SerialPort("COM3");
	mySerialPort->BaudRate = 19200;
	mySerialPort->Open();

	for (int i = 1; i < (128 - screenheight); i++)
	{
		int bs = 0;
		bs = (messages[i].at(1) + messages[i].at(2) + messages[i].at(3));								//test to see if line contains data 						

		if (bs != 96)
		{
			for (int k = 0; k < screenlength; k++)
			{
				for (int j = 0; j < screenheight; j++)
				{
					//printf("%s \n", messages[(i + j)].c_str());
					unsigned char *tline = new unsigned char[messages[(i + j)].length() + 1];			//create and fill unsigned char array
					strcpy((char *)tline, messages[(i + j)].c_str());

					cli::array<unsigned char> ^screenline =												//needs to be exactly 20 chars long 
					{
						tline[1],
						tline[2],
						tline[3],
						tline[4],
						tline[5],
						tline[6],
						tline[7],
						tline[8],
						tline[9],
						tline[10],
						tline[11],
						tline[12],
						tline[13],
						tline[14],
						tline[15],
						tline[16],
						tline[17],
						tline[18],
						tline[19],
						tline[20]
					};
					//printf("break \n");
					mySerialPort->Write(screenline												//send line to screen 
						, 0																		// We should specify buffer offset and length
						, screenline->Length);

					Sleep(scrolldelay);															//delay between screen refreshes	
					messages[(i + j)] = movetooleft(messages[(i + j)]);							//move the data one position left to give apearance of scrolling 
					messages[(i + j)] = messages[(i + j)] + " ";
				}
				cli::array<unsigned char> ^command = { 0xFE, 0x48 };							//sends cursor to start 

				//go to start 
				mySerialPort->Write(command
					, 0																			// We should specify buffer offset and length
					, command->Length);
				//printf("break \n");
			}
		}


		i = (i + (screenheight - 1));															//skip ahead to the next block of messages 
	}

	mySerialPort->Close();																		//half serial to avoid crashed upon next use 
	return 1;
}

void getcpuusage(string messages[], int messagecounter)
{
	typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
	{
		LARGE_INTEGER IdleTime;
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER Reserved1[2];
		ULONG Reserved2;
	} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;
	typedef struct _SYSTEM_BASIC_INFORMATION {
		ULONG Reserved;
		ULONG TimerResolution;
		ULONG PageSize;
		ULONG NumberOfPhysicalPages;
		ULONG LowestPhysicalPageNumber;
		ULONG HighestPhysicalPageNumber;
		ULONG AllocationGranularity;
		ULONG_PTR MinimumUserModeAddress;
		ULONG_PTR MaximumUserModeAddress;
		KAFFINITY ActiveProcessorsAffinityMask;
		CCHAR NumberOfProcessors;
	} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;
	//	SYSTEM_INFO  sysinf;
	int nWaitSec = 2;
	typedef DWORD(WINAPI * PNTQUERYSYSYTEMINFORMATION)(DWORD info_class, void *out, DWORD size, DWORD *out_size);
	PNTQUERYSYSYTEMINFORMATION pNtQuerySystemInformation = NULL;

	pNtQuerySystemInformation = (PNTQUERYSYSYTEMINFORMATION)GetProcAddress(GetModuleHandle(L"NTDLL.DLL"), "NtQuerySystemInformation");
	SYSTEM_BASIC_INFORMATION sbi;
	SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION * spi;

	DWORD returnlength;
	DWORD status = pNtQuerySystemInformation(SystemBasicInformation, &sbi,
		sizeof(SYSTEM_BASIC_INFORMATION), &returnlength);

	spi = new SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION[sbi.NumberOfProcessors];

	memset(spi, 0, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*sbi.NumberOfProcessors);

	status = pNtQuerySystemInformation(SystemProcessorPerformanceInformation, spi,
		(sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*sbi.NumberOfProcessors), &returnlength);
	int numberOfCores = returnlength / sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION);

	//printf("Number of cores: %d\n", numberOfCores);
	messages[messagecounter] = "  Number of cores:" + to_string(numberOfCores);
	messagecounter++;
	static ULARGE_INTEGER  ul_sys_idle_old[32];
	static ULARGE_INTEGER  ul_sys_kernel_old[32];
	static ULARGE_INTEGER  ul_sys_user_old[32];

	float          usage = 0;
	float          usageAccum = 0;

	//printf("Wait for %d seconds\n", nWaitSec);
	messages[messagecounter] = "  Wait for " + to_string(nWaitSec) + " seconds";
	messagecounter++;
	Sleep(nWaitSec * 1000);
	status = pNtQuerySystemInformation(SystemProcessorPerformanceInformation, spi,
		(sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*numberOfCores), &returnlength);
	usageAccum = 0;
	for (int ii = 0; ii < numberOfCores; ii++)
	{
		ULARGE_INTEGER        ul_sys_idle;
		ULARGE_INTEGER        ul_sys_kernel;
		ULARGE_INTEGER        ul_sys_user;

		ul_sys_idle.QuadPart = spi[ii].IdleTime.QuadPart;
		ul_sys_kernel.QuadPart = spi[ii].KernelTime.QuadPart;
		ul_sys_user.QuadPart = spi[ii].UserTime.QuadPart;

		ULONGLONG kernelTime = (ul_sys_kernel.QuadPart - ul_sys_kernel_old[ii].QuadPart);
		ULONGLONG usertime = (ul_sys_user.QuadPart - ul_sys_user_old[ii].QuadPart);
		ULONGLONG idletime = (ul_sys_idle.QuadPart - ul_sys_idle_old[ii].QuadPart);

		ULONGLONG proctime = kernelTime + usertime - idletime;

		ULONGLONG totaltime = kernelTime + usertime;

		usage = (float)(proctime * 100) / totaltime;
		usageAccum += usage;
		//printf("Core: %u: Usage: %f%%\n", ii + 1, usage);
		messages[messagecounter] = "  Core: " + to_string((ii + 1)) + " Usage: " + to_string(usage) + "%";
		messagecounter++;
	}
	usageAccum /= numberOfCores;
	//printf("Average for the last %d seconds: %f", nWaitSec, usageAccum);
	messages[messagecounter] = "  Average for the last " + to_string(nWaitSec) + " seconds:" + to_string(usageAccum);
	messagecounter++;
	delete[] spi;
}

int main()
{
	int scrollvalue;
	int screenlength = 20;
	int screenheight = 2;
	int scrolldelay = 250;
	string messages[256];


	while (true)
	{
		int messagecounter = 1;

		//get messages 
		getcpuusage(messages, messagecounter);

		//make all lines in array min of 50 char  
		makelengthfifty(messages);

		//send info to screen 
		sendtooscreen(messages, screenlength, screenheight, scrolldelay);
		Sleep(scrolldelay);
		//printf("Break ------------------------------------------------------------------------\n");
	}

	return 0;
}