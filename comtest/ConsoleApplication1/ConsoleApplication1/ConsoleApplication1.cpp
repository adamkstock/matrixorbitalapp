#include <iostream>
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
using namespace std;
#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;

int main() {
	// Managed array


	SerialPort^ mySerialPort = gcnew SerialPort("COM3");
	mySerialPort->BaudRate = 19200;
	mySerialPort->Open();
	for (int i = 0; i < 256; i++)
	{
		// some work with values goes here...
		cli::array<unsigned char> ^string_to_send = { 'u', 'w', 'u', '%d', i };
		cli::array<unsigned char> ^command = { 0xFE, 0x48 };
		cli::array<unsigned char> ^number = { i };

		char* num_str = (char*)malloc(256);
	
		sprintf(num_str, "%d", i);

		// We should specify buffer offset and length
		mySerialPort->Write(string_to_send
			, 0
			, string_to_send->Length);
		Sleep(1000);
		printf("sent %d\n", i);
	}
}