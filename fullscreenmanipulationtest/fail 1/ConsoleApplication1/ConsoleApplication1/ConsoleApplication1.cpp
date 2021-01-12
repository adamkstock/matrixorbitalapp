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

void makelengthfifty(string messages[])
{
	//make all strings 50 chars long
	for (int i = 0; i < 128; i++)
	{
		while (messages[i].length() < 49)
		{
			messages[i] = messages[i] + " ";
		}
		messages[i] = messages[i] + '\0';
	}
}

int sendtooscreen(string messages[], int screenlength, int screenheight, int scrolldelay)
{
	//setup the com port 
	SerialPort^ mySerialPort = gcnew SerialPort("COM3");
	mySerialPort->BaudRate = 19200;
	mySerialPort->Open();

	string internalline[128];

	for (int i = 1; i < (128 - screenheight); i++)
	{
		for (int k = 0; k < screenlength; k++)
		{
			for (int j = 0; j < screenheight; j++)
			{
				printf("%s \n", messages[(i + j)].c_str());
				unsigned char *tline = new unsigned char[messages[(i + j)].length() + 1];
				strcpy((char *)tline, messages[(i + j)].c_str());

				cli::array<unsigned char> ^screenline =										//needs to be exactly 20 chars long 
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
				
				mySerialPort->Write(screenline
					, 0																		// We should specify buffer offset and length
					, screenline->Length);

				Sleep(scrolldelay);
				messages[(i + j)] = movetooleft(messages[(i + j)]);
				messages[(i + j)] = messages[(i + j)] + " ";
			}
			cli::array<unsigned char> ^command = { 0xFE, 0x48 };							//sends cursor to start 

			//go to start 
			mySerialPort->Write(command
				, 0																			// We should specify buffer offset and length
				, command->Length);
			printf("break \n");
		}
		i = (i + (screenheight - 1));
	}

	mySerialPort->Close();
	return 1;
}


int main()
{
	int scrollvalue;
	int screenlength = 20;
	int screenheight = 2;
	int scrolldelay = 100;
	string messages[256];

	//get intial lines 
	for (int i = 1; i < 128; i++)
	{
		messages[i] = "this is test line - " + to_string(i);	//this just adds test data 
	}

	//make all lines in array min of 50 char  
	makelengthfifty(messages);

	//send info to screen 
	sendtooscreen(messages, screenlength, screenheight, scrolldelay);
	Sleep(scrolldelay);
	printf("Break ------------------------------------------------------------------------\n");

	return 0;
}	