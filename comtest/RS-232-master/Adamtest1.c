#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"



int main()
{
  int i=0,
      cport_nr=2,        
      bdrate=19200;       

  char mode[]={'8','N','1',0},
       str[2][512];


  strcpy(str[0], "Adams test code \n");

  strcpy(str[1], "Hello Matrix screen \n");

  if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
  {
    printf("Can not open comport\n");

    return(0);
  }

  while(1)
  {
    RS232_cputs(cport_nr, str[i]);

#ifdef _WIN32
    Sleep(1000);
#else
    usleep(1000000);  /* sleep for 1 Second */
#endif

    printf("sent: %i\n", (i + 1));

    i++;

    i %= 2;
  }

  return(0);
}

