

#include    <pic18.h>
#include	"Analog.h"





void	InitAD(void)
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

    bAn0_Updated = 0;
    bAn1_Updated = 0;
    bAn2_Updated = 0;
    bAn3_Updated = 0;
    bAn4_Updated = 0;
    bAn5_Updated = 0;
    bAn6_Updated = 0;
    bAn7_Updated = 0;

	AdChSel = 1; // 채널 초기값
}




void ADRead(void)
{
    unsigned long LongBuf;
    unsigned int i;

//	i=819; // 5 V 전압 기준일 때 !

    i = 1240; // 3.3 V 전압 기준일 떄 !
    LongBuf = (unsigned long)InPutAD;
    LongBuf = (LongBuf * 100);	//10mV 단위.
    LongBuf = (LongBuf / i);
    AdValue[AdChSel] = (unsigned int)(LongBuf);
    switch (AdChSel)
    {
    case    1: // AN1 : V_IN
        bAn1_Updated = TRUE;
        CHS2 = 0;
        CHS1 = 1;
        CHS0 = 0;
        AdChSel = 2;
        break;		
    case    2: // AN2 : A_IN , 출력 전류 값 !!
        bAn2_Updated = TRUE;
        CHS2 = 0;
        CHS1 = 1;
        CHS0 = 1;
        AdChSel = 3;
        break;		
    case    3: // AN3 : VR(볼륨)1 값 읽기 용(High)  낮일 때 ! 
        bAn3_Updated = TRUE;
        CHS2 = 1;
        CHS1 = 0;
        CHS0 = 0;
        AdChSel = 4;
        break;
    case    4: // AN4 : VR(볼륨)2 값 읽기 용(Low)  밤일 때 !
        bAn4_Updated = TRUE;
        CHS2 = 0;
        CHS1 = 0;
        CHS0 = 1;
        AdChSel = 1;
        break;
    default:
        CHS2 = 0;
        CHS1 = 0;
        CHS0 = 1;
        AdChSel = 1;
        break;
    }
}



bit	IsUdtAd(UINT* arInPut_mV, UCHAR* arIs_AdUpd, UCHAR AdChSel)
{
	unsigned long AvrAD;
	
    if (bAdConversion)
    {
		if ((bSetSwPushOK_Day) || (bSetSwPushOK_Night))		nADSUM = 20;
		else 												nADSUM = 10;
		
        SumAD = SumAD + (unsigned long)ADBuf; // 12비트 AD 
		SumCnt++;

        if (SumCnt >= nADSUM)
        {
            if (SumAD > 0)
            {
                AvrAD = (unsigned long)(SumAD / SumCnt);
            }
            else
            {
                AvrAD = 0;
            }

			arInPut_mV[AdChSel] = (unsigned int)((AvrAD * 1000) / 819); // 12비트 AD 기준 최대 5V에서 AD 값을 mV로 환산 !!! 
			arIs_AdUpd[AdChSel] = TRUE;
			
            SumAD = 0;
            SumCnt = 0;
			return TRUE;       
        }
        bAdConversion = FALSE;
        DONE = 1;
    }

    return(FALSE);
}


void GetMyAD(void)
{
	UCHAR ch;

	static unsigned long int Sum[ADCH_MAX] = 0;
	static unsigned int 	 Cnt[ADCH_MAX] = 0;

	for (ch=0; ch<ADCH_MAX; ch++)
	{
		if (arIs_AdUpd[ch])
		{
			switch (ch)
			{
			case 0:
				break;
			case 3: // 낮 일 때 
				Sum[ch] = Sum[ch] + (unsigned long int)arInPut_mV[ch];
				Cnt[ch]++;

				if (Cnt[ch] >= 10)
				{
					stApl[SET_DAY].SetA = (unsigned int)(Sum[ch] / Cnt[ch]); // 낮 셋팅 값 저장 (0)
					Sum[ch] = 0;
					Cnt[ch] = 0;
				}
				break;				
			case 4: // 밤 일 때 	
				Sum[ch] = Sum[ch] + (unsigned long int)arInPut_mV[ch];
				Cnt[ch]++;

				if (Cnt[ch] >= 10)
				{
					stApl[SET_NIGHT].SetA = (unsigned int)(Sum[ch] / Cnt[ch]); // 밤 셋팅값 저장 (2)
					Sum[ch] = 0;
					Cnt[ch] = 0;
				}
				break;
			case 2:
				CurA_IN_mV = arInPut_mV[ch];
				bCurA_IN_mVUpd = TRUE;
				break;
			case 1:
				CurV_IN = arInPut_mV[ch];
				break;
			default:
				break;
			}

			for (ch=0; ch<ADCH_MAX; ch++)	
				arIs_AdUpd[ch] = FALSE;
			break;
		}
	}	
}


UCHAR ChangeAdChSel(UCHAR AdSel, tag_CurDay ch)
{	
    switch (AdSel)
    {
    case 1: // V_IN 
        AdSel = 2;
        break;
    case 2: // A_IN 
        AdSel = ch;
        break;
    case 3: // 낮 셋팅 스위치 볼륨 
        AdSel = 1;
        break;
    case 4: // 밤 셋팅 스위치 볼륨 
        AdSel = 1;
        break;		
    default:
		AdSel = 1;		        
        break;
    }
	return AdSel;
}

void Set_AdCh(UCHAR AdSel)
{
	switch (AdSel)
	{
	case 0:
		CHS4 = 0;
		CHS3 = 0;
        CHS2 = 0;
        CHS1 = 0;
        CHS0 = 0;
		break;
	case 1:
		CHS4 = 0;
		CHS3 = 0;
        CHS2 = 0;
        CHS1 = 0;
        CHS0 = 1;		
		break;
	case 2:
		CHS4 = 0;
		CHS3 = 0;
        CHS2 = 0;
        CHS1 = 1;
        CHS0 = 0;		
		break;
	case 3:
		CHS4 = 0;
		CHS3 = 0;
        CHS2 = 0;
        CHS1 = 1;
        CHS0 = 1;		
		break;
	case 4:
		CHS4 = 0;
		CHS3 = 0;
        CHS2 = 1;
        CHS1 = 0;
        CHS0 = 0;		
		break;
	default:
		CHS4 = 0;
		CHS3 = 0;
        CHS2 = 0;
        CHS1 = 0;
        CHS0 = 0;		
		break;
	}
}




