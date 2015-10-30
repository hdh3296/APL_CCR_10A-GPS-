

#include    <pic18.h>


#define	AD_SUMCNT		10
#define	nAD_MAX_CHANEL	7

#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif


bit	bAn0_Update = 0;
bit	bAn1_Update = 0;
bit	bAn2_Update = 0;
bit	bAn3_Update = 0;
bit	bAn4_Update = 0;
bit	bAn5_Update = 0;
bit	bAn6_Update = 0;
bit	bAn7_Update = 0;

bit	bAdConversion = 0;

unsigned	long    SumAD	= 0;
unsigned 	long    InPutAD = 0;
unsigned 	int	    ADBuf 	= 0;
unsigned	int	    SumCnt	= 0;
unsigned    int   	AdChSel	= 0;


unsigned    int     AdValue[nAD_MAX_CHANEL]; // 채널별 Ad 값 저장 버퍼 



void	AnalogInit(void)
{
    ADCON0 = 0x01;	//ADON = 1;
    ADCON1 = 0x80;  //event= timer1
//	ADCON1=0x0;    //event= timer1

    ADFM = 1;		  						// period is an 8 bit value, right justify to get result from ADRESH
    ADCS2 = 1;  ADCS1 = 1; ADCS0 = 0;  	// 1Tad = 2 usec >= 1.6usec
    ACQT2 = 1;  ACQT1 =  0; ACQT0 = 0; 	// acquisition time >= 16~20 usec

    ANCON0 = 0x0f;  // an0-an4
    ANCON1 = 0x0;		// digital

    ADIP = 0;
    ADIF = 0;
    ADIE = 1;

    DONE = 1;

    bAn0_Update = 0;
    bAn1_Update = 0;
    bAn2_Update = 0;
    bAn3_Update = 0;
    bAn4_Update = 0;
    bAn5_Update = 0;
    bAn6_Update = 0;
    bAn7_Update = 0;

	AdChSel = 3; // 채널 초기값 : 3 채널  
}




void ADRead(void)
{
    unsigned long LongBuf;
    unsigned int i;

//	i=819;

    i = 1240;
    LongBuf = (unsigned long)InPutAD;
    LongBuf = (LongBuf * 100);	//10mV 단위.
    LongBuf = (LongBuf / i);
    AdValue[AdChSel] = (unsigned int)(LongBuf);
    switch (AdChSel)
    {
    case    3: // AN3 : VR(볼륨)1 값 읽기 용(High) 
        bAn3_Update = TRUE;
        CHS2 = 1;
        CHS1 = 0;
        CHS0 = 0;
        AdChSel = 4;
        break;
    case    4: // AN4 : VR(볼륨)2 값 읽기 용(Low) 
        bAn4_Update = TRUE;
        CHS2 = 0;
        CHS1 = 1;
        CHS0 = 1;
        AdChSel = 3;
        break;
    default:
        CHS2 = 0;
        CHS1 = 1;
        CHS0 = 1;
        AdChSel = 3;
        break;
    }
}



unsigned char	ADConversionChk(void)
{
    if (bAdConversion)
    {
        SumCnt++;
        SumAD = SumAD + (unsigned long)ADBuf;
        if (SumCnt >= AD_SUMCNT)
        {
            if (SumAD > 0)
            {
                InPutAD = (unsigned long)(SumAD / SumCnt);
            }
            else
            {
                InPutAD = 0;
            }
            ADRead();
            SumCnt = 0;
            SumAD = 0;
        }
        bAdConversion = FALSE;
        DONE = 1;
    }

    return(0);
}








