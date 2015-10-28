

#include    <pic18.h>

/*
#include        "..\..\system_com\memory_map.h"
#include        "setup.h"
#include        "fontout.h"
#include        "iodef.h"
#include        "keysort.h"
*/


volatile const unsigned char information[12]={200,135,0,0,0,0,0,0,0,0,0};	/*this is the variable in FLASH where the old text resides*/
unsigned char new_value[]={0,0,0,0,0,0,0,0,0,0,0};	             		/*unlike the old_text this is not a CONST -> stored in data RAM */
far unsigned char * source_ptr = (far unsigned char *)new_value;        /*pointers to data*/
far unsigned char * dest_ptr = (far unsigned char *)information;
unsigned char size = 11;						


/*
unsigned    char    SrcAddress;
unsigned    char    LocalNumber=0;
unsigned    char    offset_flr=0;
unsigned    char    ButtonType=0;
unsigned    char    CallOffset=0;

bit   ChangeSetup=0;
bit   UpKeyOn=0;
bit   UpKeyLong=0;
bit   UpKeyOneTouch=0;
bit   DnKeyOn=0;
bit   DnKeyLong=0;
bit   DnKeyOneTouch=0;
bit   CmpSetBit=0;
bit   MaskSetBit=0;
bit   bToggleOn=0;
bit   bCurOpenKey=0;
bit   bUserLamp4=0;
bit   bHibHpiSel=0;
bit   bMainSubDoor=0;
*/

unsigned    char	SetNightVolt=200;
unsigned    char	SetNightDayVolt=135;


void    ReadFlashSetup(void)
{
    unsigned char i;

    for(i=0;i<size;i++){
        new_value[i]=information[i];
    }
}


void    LoadSetupValue(void)
{
	ReadFlashSetup();
    SetNightVolt=information[0];
    SetNightDayVolt=information[1];


/*
    LocalNumber=information[0];

    offset_flr=information[3];
    if(offset_flr > 2)   offset_flr = 0;     

    ButtonType=information[4];

    Company=information[5];


	bToggleOn=0;
    if(information[6] & 0x01)	bToggleOn=1;

	bCurOpenKey=0;
    if(information[6] & 0x02)	bCurOpenKey=1;

	bUserLamp4=0;
    if(information[6] & 0x04)	bUserLamp4=1;


	bHibHpiSel=0;
    if(information[6] & 0x08)	bHibHpiSel=1;

	bMainSubDoor=0;
    if(information[6] & 0x10)	bMainSubDoor=1;

	CallOffset=information[7];
*/

}



/*
void    Display(unsigned char i,unsigned char j,unsigned char k)
{

    if(MainTimer>5){
        MainTimer=0;
        FDsp=!FDsp;
#if defined(__DSP_DOT)
        SetupDisplay(i,j);
#else     
        SetupDisplayES15(i,k);
#endif     
    }         
}
*/


void    CompanyWrite(void) 
{
    flash_write(source_ptr,size,dest_ptr);
//   asm("reset");
}



/*
void	SetClearBit(unsigned char *TargetBuf,unsigned char value,unsigned char bitnm)
{
	unsigned char bitvalue;
	bitvalue=0x01;
	bitvalue=(bitvalue << bitnm);

	if( *TargetBuf & bitvalue){
		if(value==1){
			*TargetBuf=( *TargetBuf & ~bitvalue);	
			ChangeSetup=1;
		}				
	}
	else{
		if(value>1){
			*TargetBuf=( *TargetBuf | bitvalue);	
			ChangeSetup=1;
		}				
	}
}



void	SetClearByte(unsigned char *TargetBuf,unsigned char value)
{

	if(value > 0){
		value=(value - 1 );
		if(value != *TargetBuf){
			*TargetBuf=value;
			ChangeSetup=1;
		}
	}

}


void	CmdSetupChk(unsigned char id)
{		

    unsigned char	tmpb;
    unsigned int    IdPt;
   
	IdPt=IsBufferPt(id);

    if((RcvBuf[IdPt+DSP1] == '9') && (RcvBuf[IdPt+DSP2] == '1') && !(RcvBuf[IdPt+1] & S1_AUTO) && ((RcvBuf[IdPt+SL_mCarOpCl] & 0xc0)== 0xc0) ){
		tmpb= (RcvBuf[IdPt+SL_mCarOpCl] & 0x03);
		SetClearBit(&new_value[6],tmpb,0);

		tmpb= (RcvBuf[IdPt+SL_mCarOpCl] & 0x0c);
		tmpb=(tmpb >> 2);
		SetClearBit(&new_value[6],tmpb,1);

		tmpb= (RcvBuf[IdPt+SL_mCarOpCl] & 0x30);
		tmpb=(tmpb >> 4);
		SetClearBit(&new_value[6],tmpb,2);


		tmpb= (RcvBuf[IdPt+SL_mCarKey25] & 0x03);
		SetClearBit(&new_value[6],tmpb,3);

		tmpb= (RcvBuf[IdPt+SL_mCarKey25] & 0x0c);
		tmpb=(tmpb >> 2);
		SetClearBit(&new_value[6],tmpb,4);


		SetClearByte( &new_value[7],RcvBuf[IdPt+SL_mCarKey1]);
		SetClearByte( &new_value[4],RcvBuf[IdPt+SL_mCarKey9]);
		SetClearByte( &new_value[3],RcvBuf[IdPt+SL_mCarKey17]);
		SetClearByte( &new_value[0],(id+1));
	}
 }
*/

