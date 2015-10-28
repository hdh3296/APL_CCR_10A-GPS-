


#include    <pic18.h>

#include    "Com1_Port.h"
#include    "serial.h"


unsigned    int  Crc;  


void    Crc_Calulate(unsigned int crcdata)
{
	register unsigned int    i;

   Crc=Crc ^ (crcdata & 0x00ff);
   for(i=0;i<=7;i++){
      if( (Crc & 0x0001) == 0x0001)    Crc=(Crc >> 1) ^ 0xA001;
      else Crc=Crc >> 1;
   }
}





