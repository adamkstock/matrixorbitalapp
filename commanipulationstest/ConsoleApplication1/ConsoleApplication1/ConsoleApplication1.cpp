#include <iostream>
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <string>

#using <System.dll>

using namespace std;
using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;

int sendtooscreen(string top, string bottom, int screenlength, int scrolldelay) 
{

	while (top.length() < 50)
	{
		top = top + " ";
	}
	top = top + '\0';

	printf("sent- %s -end\n", top);


	unsigned char *convtop = new unsigned char[top.length() + 1];
	strcpy((char *)convtop, top.c_str());

	while (bottom.length() < 50)
	{
		bottom = bottom + " ";
	}
	bottom = bottom + '\0';


	unsigned char *convbottom = new unsigned char[bottom.length() + 1];
	strcpy((char *)convbottom, bottom.c_str());

	//setup the com port 
	SerialPort^ mySerialPort = gcnew SerialPort("COM3");
	mySerialPort->BaudRate = 19200;
	mySerialPort->Open();

	//Values to go to screen 
	cli::array<unsigned char> ^topscreenline =									//needs to be exactly 20 chars long 
	{
		convtop[1],
		convtop[2],
		convtop[3],
		convtop[4],
		convtop[5],
		convtop[6],
		convtop[7],
		convtop[8],
		convtop[9],
		convtop[10],
		convtop[11],
		convtop[12],
		convtop[13],
		convtop[14],
		convtop[15],
		convtop[16],
		convtop[17],
		convtop[18],
		convtop[19],
		convtop[20]
	};			

	//printf("sent- %s -end\n", topscreenline);

	cli::array<unsigned char> ^bottomscreenline =								//needs to be exactly 20 chars long 
	{
		convbottom[1],
		convbottom[2],
		convbottom[3],
		convbottom[4],
		convbottom[5],
		convbottom[6],
		convbottom[7],
		convbottom[8],
		convbottom[9],
		convbottom[10],
		convbottom[11],
		convbottom[12],
		convbottom[13],
		convbottom[14],
		convbottom[15],
		convbottom[16],
		convbottom[17],
		convbottom[18],
		convbottom[19],
		convbottom[20]
	};

	cli::array<unsigned char> ^command = { 0xFE, 0x48 };							//sends cursor to start 

	//go to start 
	mySerialPort->Write(command
		, 0																			// We should specify buffer offset and length
		, command->Length);															// We should specify buffer offset and length
	printf("sent- Command for new line -end\n");

	//send top line
	mySerialPort->Write(topscreenline
		, 0																			// We should specify buffer offset and length
		, topscreenline->Length);													// We should specify buffer offset and length
	printf("sent- %s -end\n", topscreenline);

	Sleep(scrolldelay);

	//send second line 
	mySerialPort->Write(bottomscreenline
		, 0																			// We should specify buffer offset and length
		, bottomscreenline->Length);												// We should specify buffer offset and length
	printf("sent %s -end\n", bottomscreenline);

	Sleep(scrolldelay);

	mySerialPort->Close();
	
	return 0;
}

string movetooleft(string line)
{
	string data = line;
	int length = data.length();

	for (int i = 1; i < length; i++)
	{
		data[i] = data[(i + 1)];
	}
	data[(length - 1)] = ' ';

	return data;
}

int main() 
{
	int scrollvalue;
	int screenlength = 20;
	int scrolldelay = 10;

	
	while (true)
	{
		//get intial lines 
		string top = " this is a ";
		string bottom = " test of the array";

		//send info to screen and scroll 
		for (scrollvalue = screenlength; scrollvalue > 0; scrollvalue--)
		{
			sendtooscreen(top, bottom, screenlength, scrolldelay);
			top = movetooleft(top);
			bottom = movetooleft(bottom); 
			Sleep(scrolldelay);
		}
	}

	return 0;
}