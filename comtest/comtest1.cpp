#include <stdio.h>
#include <string.h>

bool WriteComPort(CString PortSpecifier, CString  data);


int main() {
	WriteComPort(CString 3, CString test)
	return 0;
}

bool WriteComPort(CString PortSpecifier, CString  data)
{
	DCB dcb;
	DWORD byteswritten;
	HANDLE hPort = CreateFile
	(
		PortSpecifier,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	
	if (!GetCommState(hPort,&dcb))
		return false;
		
	dcb.BaudRate = CBR_19200; //19200 Baud
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop
	if (!SetCommState(hPort,&dcb))
		return false;
		
	bool retVal = WriteFile(hPort,data,1,&byteswritten,NULL);
	CloseHandle(hPort); //close the handle
	return retVal;
} 