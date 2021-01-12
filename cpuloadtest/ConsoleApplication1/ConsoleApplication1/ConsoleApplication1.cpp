#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#define SystemProcessorPerformanceInformation 0x8
#define SystemBasicInformation    0x0
int _tmain(int argc, _TCHAR* argv[])
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

	if (argc < 2)
	{
		printf("Please specify waiting time in seconds\n");
		return -1;
	}
	int nWaitSec = _wtoi(argv[1]);
	if (nWaitSec <= 0)
	{
		printf("Waiting interval in seconds should be positive integer\n");
		return -1;
	}
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

	printf("Number of cores: %d\n", numberOfCores);
	static ULARGE_INTEGER  ul_sys_idle_old[32];
	static ULARGE_INTEGER  ul_sys_kernel_old[32];
	static ULARGE_INTEGER  ul_sys_user_old[32];

	float          usage = 0;
	float          usageAccum = 0;

	printf("\n\nWait for %d seconds\n", nWaitSec);
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
		printf("Core        : %u: Usage      : %f%%\n", ii + 1, usage);
	}
	usageAccum /= numberOfCores;
	printf("----------------\nAverage for the last %d seconds: %f", nWaitSec, usageAccum);
	delete[] spi;
	return 0;
}