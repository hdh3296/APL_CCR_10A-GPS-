

#include    <pic18.h>


#define	AD_SUMCNT		10
#define	AD_MAX_CHANEL	4

#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif


bit	An0_Update = 0;
bit	An1_Update = 0;
bit	An2_Update = 0;
bit	An3_Update = 0;
bit	An4_Update = 0;
bit	An5_Update = 0;
bit	An6_Update = 0;
bit	An7_Update = 0;

bit	bAdConversion = 0;

unsigned	long    SumAD	= 0;
unsigned 	long    InPutAD = 0;
unsigned 	int	    ADBuf 	= 0;
unsigned	int	    SumCnt	= 0;
unsigned    char    AdChSel	= 0;


unsigned    int     AdValue[AD_MAX_CHANEL]; // 채널별 Ad 값 저장 버퍼 



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

    An0_Update = 0;
    An1_Update = 0;
    An2_Update = 0;
    An3_Update = 0;
    An4_Update = 0;
    An5_Update = 0;
    An6_Update = 0;
    An7_Update = 0;
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
    case    0:  	// AN0
        An0_Update = 1;
        AdChSel = 1;
        if (AdChSel < AD_MAX_CHANEL)
        {
            CHS2 = 0;
            CHS1 = 0;
            CHS0 = 1;
        }
        break;
    case    1:		// AN1
        An1_Update = 1;
        AdChSel = 2;
        if (AdChSel < AD_MAX_CHANEL)
        {
            CHS2 = 0;
            CHS1 = 1;
            CHS0 = 0;
        }
        break;
    case    2:		// AN2
        An2_Update = 1;
        AdChSel = 3;
        if (AdChSel < AD_MAX_CHANEL)
        {
            CHS2 = 0;
            CHS1 = 1;
            CHS0 = 1;
        }
        break;
    case    3:		// AN3
        An3_Update = 1;
        CHS2 = 0;
        CHS1 = 0;
        CHS0 = 0;
        AdChSel = 0;


        /*
        			AdChSel=4;
        			if(AdChSel < AD_MAX_CHANEL){
        				CHS2=1;
        				CHS1=0;
        				CHS0=0;
        			AdChSel=0;
        			}
        */
        break;
    case    4:		// AN4
        An4_Update = 1;
        AdChSel = 5;
        if (AdChSel < AD_MAX_CHANEL)
        {
            CHS2 = 0;
            CHS1 = 0;
            CHS0 = 0;
            AdChSel = 0;
        }
        break;
    default:
        CHS2 = 0;
        CHS1 = 0;
        CHS0 = 0;
        AdChSel = 0;
        break;
    }

    /*
    	if( AdChSel >= AD_MAX_CHANEL){
    		CHS2=0;
    		CHS1=0;
    		CHS0=0;
    		AdChSel=0;
    	}
    */
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








