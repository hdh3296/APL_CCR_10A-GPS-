




#include    <pic18.h>

#include	"serial.h"
#include   	"Com1_Port.h"
#include   	"Com2_Port.h"
#include   	"Pwm1.h"
#include   	"analog.h"
#include   	"setup.h"
#include	"apl_ccr_10a_main.h"






void    InitPort(void)
{
    TRISA = 0xff;
    TRISB = 0xff;
    TRISC = 0xff;
    TRISD = 0xff;
    TRISE = 0xff;

    LATA = 0xff;
    LATB = 0xff;
    LATC = 0xff;
    LATD = 0xff;
    LATE = 0xff;

    NoUse_CDS_TRIS = 1;
    V_IN_TRIS = 1;
    A_IN_TRIS = 1;
    ISET_HI_TRIS = 1;
    ISET_LO_TRIS = 1;

    NIGHT_IN_TRIS = 1;
    SAVE_HI_TRIS = 1;
    SAVE_LO_TRIS = 1;
    NoUse_MODE3_TRIS = 1;
    NoUse_MODE4_TRIS = 1;
    NoUse_MODE5_TRIS = 1;
    PGC_TRIS = 1;
    PGD_TRIS = 1;

    NoUse_MODE6_TRIS = 1;
    NoUse_MODE7_TRIS = 1;
    NoUse_FORCE_TRIS = 0;
    NoUse_INVALID_TRIS = 0;
    NoUse_RC4_TRIS = 1;
    LED_RUN2_TRIS = 0;
    NoUse_TX1_TRIS = 0;
    NoUse_RX1_TRIS = 1;

    LED_ON_TRIS = 0;
    NoUse_EX2_ON_TRIS = 0;
    NoUse_GPS_ON_TRIS = 0;
    LED_RUN0_TRIS = 0;
    PWM_TRIS = 0;
    LED_RUN1_TRIS = 0;
    NoUse_TX2_TRIS = 0;
    TX_DP_TRIS = 1;

    PPS_TRIS = 1;
    NoUse_OPTION_TRIS = 1;
    NoUse_M3_TRIS = 1;



    _LAMP_ON = 0;
    _PWM = 0;
    NoUse_GPS_ON = 1;
    NoUse_FORCE = 0;  // ????
    NoUse_INVALID = 1;
    NoUse_EX2_ON = 0;
    _LED_GpsGoodState = 1;	// 1 : Led Off
    _LED_AplLampState = 1;	// 1 : Led Off
    _LED_NIG = 1;			// 1 : Led Off
    NoUse_TX2 = 1;
    NoUse_TX1 = 1;
}


void    UserBaudRate(void)
{
    unsigned char val;
    val = 0;

    TRISE0 = 1;
    TRISE1 = 1;
    TRISE2 = 1;
    if (!RE0)	val = (val | 0x01);
    if (!RE1)	val = (val | 0x02);
    if (!RE2)	val = (val | 0x04);
    switch (val)
    {
    case	0:
        Com1BaudRate = 9600;
        break;
    case	1:
        Com1BaudRate = 19200;
        break;
    case	2:
        Com1BaudRate = 38400;
        break;
    case	3:
        Com1BaudRate = 57600;
        break;
    default:
        Com1BaudRate = 57600;
        break;
    }
}


void  Serial2Check(void)
{
    unsigned char i;

    if (Com1RxStatus == RX_GOOD)
    {
//		for(i=0;i<(Com1RxBuffer[3]+4);i++)	Can1TxBuf[i]=Com1RxBuffer[i];
//		Can1TxDataTxPointer=2;
//		Can1TxDataTotalCnt= (Com1RxBuffer[3]+4);
        Com1RxStatus = STX_CHK;
//		RXLED = !RXLED;
    }
}

/*
   			Com1TxCurCnt=0;
 			Com1TxTotalCnt=(Com1TxTotalCnt+2);
			Com1TxStart();
*/


// GPS 에서 수신된 펄스를 체크한다.
// 1초마다 High 값이 수신된다.
// High Edge 값을 체크한다.
void GpsPPS1Chk(void)
{
    if (_PPS)
    {
        if (bPPS_On == FALSE)
        {
            bPPS_Edge = TRUE;
        }
        bPPS_On = TRUE;
    }
    else
    {
        bPPS_On = FALSE;
    }
}




void LedBlinkModeInit(void)
{
    unsigned char	i;

    for (i = 0;i < LED_ONOFF_CNT;i++)
    {
        OnTime[i] = 0x0;
        LedCycle_Msec = 0;
    }
}



unsigned int ReSettingDayNigntChk(void)
{
    if (NoUse_MODE5)
    {
        SettingReadyTime = 0;
        NightDaySetTime = 0;
        bNightDay = 0;
        NightSetTime = 0;
        bNight = 0;
        _LED_NIG = 1;
        return(0);
    }


    if (!NoUse_MODE7 && NoUse_MODE6)
    {
        if ((NightSetTime > 5) && (!bNight))
        {
            bNight = 1;
            new_value[0] = AdValue[0];
            CompanyWrite();
            LoadSetupValue();
        }
        if (bNight)	_LED_NIG = 0;
    }
    else if (!NoUse_MODE6 && NoUse_MODE7)
    {
        if ((NightDaySetTime > 5) && (!bNightDay))
        {
            bNightDay = 1;
            new_value[1] = AdValue[0];
            CompanyWrite();
            LoadSetupValue();
        }
        if (bNightDay)	_LED_NIG = 0;
    }
    else
    {
        NightDaySetTime = 0;
        bNightDay = 0;

        NightSetTime = 0;
        bNight = 0;

        if (SettingReadyTime > 4)
        {
            _LED_NIG = !_LED_NIG;
            SettingReadyTime = 0;
        }
    }

    return(0);
}



/*
void ModeChk(void)
{
	unsigned char i,j,befmodesw;

	befmodesw=modesw;

	modesw=0;

	if( !NIGHT_IN)	modesw=(modesw | 0x01);
	if( !SAVE_HI)	modesw=(modesw | 0x02);
	if( !SAVE_LO)	modesw=(modesw | 0x04);
	if( !NoUse_MODE3)	modesw=(modesw | 0x08);
	if( !NoUse_MODE4)	modesw=(modesw | 0x10);


	if( (modesw & 0x1f) != (befmodesw & 0x1f)){
		LedBlinkModeInit();
		i=(modesw & 0x1f);

		switch(i){
			case	0:
				OnTime[0]  		=1000;
				LedCycle_Msec	=4000;
				break;
			case	1:
				OnTime[0]  		=1000;
				LedCycle_Msec	=5000;
				break;
			case	2:
				OnTime[0]  		=1000;
				LedCycle_Msec	=6000;
				break;
			case	3:
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				LedCycle_Msec	=6000;
				break;
			case	4:						// 7s
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				OnTime[3]  		=2000;
				OnTime[4]  		=2500;
				LedCycle_Msec	=7000;
				break;
			case	5:						// 8s
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				OnTime[3]  		=2000;
				OnTime[4]  		=2500;
				OnTime[5]  		=3000;
				OnTime[6]  		=3500;
				LedCycle_Msec	=8000;
				break;
			case	6:						// 20s-5
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				OnTime[3]  		=2000;
				OnTime[4]  		=2500;
				OnTime[5]  		=3000;
				OnTime[6]  		=3500;
				OnTime[7]  		=4000;
				OnTime[8]  		=4500;
				LedCycle_Msec	=20000;
				break;
			case	7:						// 5s-2
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				LedCycle_Msec	=5000;
				break;
			case	8:						// 10s-2
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				LedCycle_Msec	=10000;
				break;
			case	9:						// Q
				OnTime[0]  		=500;
				LedCycle_Msec	=1000;
				break;
			case	10:						// Q3-10s
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				OnTime[3]  		=2000;
				OnTime[4]  		=2500;
				LedCycle_Msec	=1000;
				break;
			case	11:						// Q6 + LF115s
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				OnTime[3]  		=2000;
				OnTime[4]  		=2500;
				OnTime[5]  		=3000;
				OnTime[6]  		=3500;
				OnTime[7]  		=4000;
				OnTime[8]  		=4500;
				OnTime[9]  		=5000;
				OnTime[10]  	=5500;
				OnTime[11]  	=6000;
				OnTime[12]  	=8000;
				LedCycle_Msec 	=15000;
				break;
			case	12:						// Q9 + 15s
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=1500;
				OnTime[3]  		=2000;
				OnTime[4]  		=2500;
				OnTime[5]  		=3000;
				OnTime[6]  		=3500;
				OnTime[7]  		=4000;
				OnTime[8]  		=4500;
				OnTime[9]  		=5000;
				OnTime[10]  	=5500;
				OnTime[11]  	=6000;
				OnTime[12]  	=6500;
				OnTime[13]  	=7000;
				OnTime[14]  	=7500;
				OnTime[15]  	=8000;
				OnTime[16]  	=8500;
				LedCycle_Msec 	=15000;
				break;
			case	13:						// Mo(A)8s
				OnTime[0]  		=500;
				OnTime[1]  		=1000;
				OnTime[2]  		=3000;
				LedCycle_Msec 	=8000;
				break;
			case	14:						// LF1_10s
				OnTime[0]  		=2000;
				LedCycle_Msec 	=10000;
				break;
			case	15:						// F
				OnTime[0]  		=10000;
				LedCycle_Msec 	=10000;
				break;
			case	17:						// isow4s
				OnTime[0]  		=1000;
				LedCycle_Msec 	=4000;
				break;
			case	18:						// isow6s
				OnTime[0]  		=1000;
				LedCycle_Msec 	=6000;
				break;
			case	19:						// isow8s
				OnTime[0]  		=1000;
				LedCycle_Msec 	=8000;
				break;
			case	20:						// isow10s
				OnTime[0]  		=1000;
				LedCycle_Msec 	=10000;
				break;
			default:
				OnTime[0]  		=0;
				LedCycle_Msec 	=10000;
				break;
		}
	}
}



void ActiveOnChk(void)
{

	bActiveOn=1;

	if(AdValue[0] >= SetNightDayVolt){
		if(DayNightTimer > 5 ){
			DayNightTimer=0;
			_LED_GpsGoodState = !_LED_GpsGoodState;
		}

		if(AdValue[0] >= SetNightVolt){
			bActiveOn=1;
			_LED_GpsGoodState=0;
		}
	}
	else{
		bActiveOn=0;
		_LED_GpsGoodState=1;
	}

	if(bAn0_Updated){
		bAn0_Updated=0;
		if(WakeupTime < 200) WakeupTime++;
	}
}



void ApaLampOnOff(void)
{
	ActiveOnChk();
	if(bActiveOn || !NoUse_MODE5){
		if(!bPwmOn)	PwmOn();

		if(bAn2_Updated && bAn3_Updated){
			bAn2_Updated=0;
 			bAn3_Updated=0;

			if(AdValue[2] < AdValue[3]){
				if(DutyCycle < 0x3ff)	DutyCycle++;
				else					DutyCycle=0x3ff;
				PwmOut();
			}
			else if(AdValue[2] > AdValue[3]){
				if(DutyCycle > 0x0)	DutyCycle--;
				else				DutyCycle=0;

				PwmOut();
			}
		}

		if(Gms60000 <= OnTime[0]){  		// led on
			_APLLAMP=1;
		}
		else if(Gms60000 <= OnTime[1]){  	// led on
			_APLLAMP=0;
		}
		else if(Gms60000 <= OnTime[2]){  	// led on
			_APLLAMP=1;
		}
		else if(Gms60000 <= OnTime[3]){  	// led on
			_APLLAMP=0;
		}
		else if(Gms60000 <= OnTime[4]){  	// led on
			_APLLAMP=1;
		}
		else if(Gms60000 <= OnTime[5]){  	// led on
			_APLLAMP=0;
		}
		else if(Gms60000 <= OnTime[6]){  	// led on
			_APLLAMP=1;
		}
		else if(Gms60000 <= OnTime[7]){  	// led on
			_APLLAMP=0;
		}
		else if(Gms60000 <= OnTime[8]){  	// led on
			_APLLAMP=1;
		}
		else if(Gms60000 <= OnTime[9]){  	// led on
			_APLLAMP=0;
		}
		else if(Gms60000 <= OnTime[10]){  	// led on
			_APLLAMP=1;
		}
		else if(Gms60000 <= OnTime[11]){  	// led on
			_APLLAMP=0;
		}
		else if(Gms60000 <= OnTime[12]){  	// led on
			_APLLAMP=1;
		}
		else if(Gms60000 <= OnTime[13]){  	// led on
			_APLLAMP=0;
		}
		else if(Gms60000 <= OnTime[14]){  	// led on
			_APLLAMP=1;
		}
		else if(Gms60000 <= OnTime[15]){  	// led on
			_APLLAMP=0;
		}
		else if(Gms60000 <= OnTime[16]){  	// led on
			_APLLAMP=1;
		}
		else{
			_APLLAMP=0;
		}

		bPwmOn=1;
		GPS_ON=0;

		_LED_AplLampState= !_APLLAMP;
	}
	else{
		if(bPwmOn){
			PwmOff();
			DutyCycle=0x3ff;
			PwmOut();
		}
		bPwmOn=0;

		PWM=0;
		_APLLAMP=0;
		GPS_ON=1;

		_LED_AplLampState=1;

		if(WakeupTime > 0){
			bAn0_Updated=0;
			CLRWDT();
			MainTimer=0;
			WakeupTime=0;
			_LED_GpsGoodState=1;
			_LED_NIG=1;

			SLEEP();
		}

	}
}

*/

/*
void SetApaLamp(void)
{
    if (bAn2_Updated) // A_IN 아날로그 값이 업데이트 됬으면 ?
    {
        bAn2_Updated = FALSE;

        if (stApl[0].bSetSwPushOK)		SetAVoltage = SetA1_Volt;
        else if (stApl[2].bSetSwPushOK)	SetAVoltage = SetA3_Volt;
        // Ad2 와 Ad3(또는 Ad4) 값을 비교하여 Pwm 듀티 값을 증가 또는 감소 한다.
        DutyCycle = GetDutyByCompareCurrent(DutyCycle, SetAVoltage, A_IN_Volt, CurDayNight);

    }
    _LAMP_ON = ON_lamp; // 실제 APL 램프 ON
    PwmOut(DutyCycle);
    bPwmOn = TRUE;
}
*/
/*
// APL Lamp 출력 함수이다.
void ApaLampOnOff(void)
{
    // bBlink_DutyOn 상태 (-On듀티상태-)에서
    // _LAMP_ON 및 PWM 출력을 내보내면 LAMP에 실제로 불이 켜진다.
    if (bBlink_DutyOn)
    {
        if (bPwmOn == FALSE)
            PwmOn();

        if (bAgoBlkLedOff)
        {
            bAgoBlkLedOff = FALSE;
            StartTimer = 0;

            //if (CurDayNight == DAY)
                //ReadVal(&SavedDutyCycle1, &SavedSetA1_Volt, Saved1Buf, &SetA1_Volt);
            //else
                //ReadVal(&SavedDutyCycle3, &SavedSetA3_Volt, Saved3Buf, &SetA3_Volt);

        }
        else
        {
            if (StartTimer > 100)
            {
                if (bAn2_Updated) // A_IN 아날로그 값이 업데이트 됬으면 ?
                {
                    bAn2_Updated = FALSE;

                    if (CurDayNight == DAY) 	SetAVoltage = SetA1_Volt;
                    else 					SetAVoltage = SetA3_Volt;
                    // Ad2 와 Ad3(또는 Ad4) 값을 비교하여 Pwm 듀티 값을 증가 또는 감소 한다.
                    DutyCycle = GetDutyByCompareCurrent(DutyCycle, SetAVoltage, A_IN_Volt, CurDayNight);
                }
            }
        }
        _LAMP_ON = ON_lamp; // 실제 APL 램프 ON
        PwmOut(DutyCycle);
        bPwmOn = TRUE;
        _LED_AplLampState = ON_runled1; // 상태 LED 깜빡 깜빡 !!!

    }
    else
    {
        if (bPwmOn)
        {
            PwmOff();
            DutyCycle = 0x3ff;
            PwmOut(DutyCycle);
        }
        bPwmOn = FALSE;
        _PWM = 0;
        _LAMP_ON = OFF_lamp;
        _LED_AplLampState = OFF_runled1;
        bAgoBlkLedOff = TRUE;
    }
}
*/

// GPS 펄스에 의한 엣지가 생기면 !
// msec 값을 999으로 변경하고
// GPS 시,분,초 변수값에 시간을 저장한다.
bit IsBlink_On(void)
{
    static bit bBlink_DutyOn;

	// 
    if (bPPS_Edge && bPPS_On)
    {
        bPPS_Edge = FALSE;
        if (rx_sec == 59) // 현재 59초 이면 ? 
        {
            Gms1 = 999; // 999 ms 로 변경 
            Gsec = rx_sec; // Gps Rx 시간 값들을 차례로 저장 
            Gmin = rx_min;
            Ghour = rx_hour;
            rx_sec = 0;
        }
    }

    Gms1++; // Timer에 의한 1ms 마다 증가 
    // ms 가 1000(1초)이면 시간 값(초,분,시)을 1씩 증가 시킨다.
    if (Gms1 >= 1000)
    {
        Gms1 = 0;
        Gsec++;
        if (Gsec >= 60)
        {
            Gsec = 0;
            Gmin++;
            if (Gmin >= 60)
            {
                Gmin = 0;
                Ghour++;
                if (Ghour >= 24)
                {
                    Ghour = 0;
                }
            }
        }
    }
	// ms 값이 0 일때 ZeroTimer값 갱신 
    if (Gms1 == 0)
    {
        ZeroTimer = (ULONG)((ULONG)Ghour * (ULONG)3600000);
        ZeroTimer = ZeroTimer + (ULONG)((ULONG)Gmin  * (ULONG)60000);
        ZeroTimer = ZeroTimer + (ULONG)((ULONG)Gsec  * (ULONG)1000);
    }
	// 현재 시간값을 ms 단위로 환산
    Gms60000 = (UINT)((ZeroTimer + (ULONG)Gms1) % (ULONG)LED_CYCLE_MSEC);

    if (Gms60000 < LED_ON_DUTY_MSEC)
    {
        bBlink_DutyOn = TRUE; // APL LAMP ON
    }
    else
    {
        bBlink_DutyOn = FALSE;  // APL LAMP off
    }

    return bBlink_DutyOn;
}

// GPS RX2 통신 값을 받아서 Com1TxBuffer로 넘겨준다.
// GPS 수신 Good 신호를 받으면, 시,분,초 변수에 각각 저장한다.
void GpsRx2DataProc(void)
{
    unsigned int i;

    if (Com2RxBuffer[18] == 'A') // GPS 수신 GOOD !
    {
	    _LED_GpsGoodState = !_LED_GpsGoodState; // GPS 수신 GOOD 상태 LED
        i = (Com2RxBuffer[7] - 0x30) * 10;
        i = (Com2RxBuffer[8] - 0x30) + i;
        rx_hour = i;

        i = (Com2RxBuffer[9]  - 0x30) * 10;
        i = (Com2RxBuffer[10] - 0x30) + i;
        rx_min = i;

        i = (Com2RxBuffer[11] - 0x30) * 10;
        i = (Com2RxBuffer[12] - 0x30) + i;
        rx_sec = i;
    }

    for (i = 0; i < Com2RxCurCnt; i++)
        Com1TxBuffer[i] = Com2RxBuffer[i];

    // msec 값 3자리를 각각 Tx버퍼에 저장
    i = (Gms1 % 100);
    Com1TxBuffer[14] = (unsigned char)((Gms1 / 100) + 0x30);
    Com1TxBuffer[15] = (unsigned char)((i / 10) + 0x30);
    Com1TxBuffer[16] = (unsigned char)((i % 10) + 0x30);

    Com2RxBuffer[18] = 0x0;

    Com1TxCurCnt = 0;
    Com1TxTotalCnt = Com2RxCurCnt;
    Com1TxStart();
}



// Blink Led On, Off 판별
bit IsInLED_ON(unsigned char bLedState, unsigned char* Timer)
{
    static bit bBlkLedOn;

    bBlkLedOn = TRUE;
    if (bLedState)	//	High
    {
        if (*Timer > 90) // High 가 90ms 유지 되면 낮이다.
            bBlkLedOn = FALSE;
    }
    else 	// Low, 60Hz 이면 밤이다. 
    {
        *Timer = 0;
        bBlkLedOn =  TRUE;
    }

    return bBlkLedOn;
}


unsigned char GetDayEveningNight(void)
{
    static bit bDayLed, bNightLed;
    unsigned char ret;

    bNightLed = IsInLED_ON(_CDS_NIGHT_IN, &InDayTimer);

    if (bNightLed)
        ret = NIGHT;	// 밤
    else
        ret = DAY;		// 낮

    return ret;

}



void ChkSetupSw(void)
{
	// 낮 스위치 
    if (_SW_SET_HI == SETSW_PUSH) // 스위치를 눌렀을 때 !!!
    {
        if (stApl[SW_DAY].SetSwCharterTimer > 50)
        {
            stApl[SW_DAY].bSetSwPushOK = TRUE;
        }
    }
    else  // 스위치를 뗐을 때 !
    {
        stApl[SW_DAY].SetSwCharterTimer = 0;
        if (stApl[SW_DAY].bSetSwPushOK)
        {
            stApl[SW_DAY].bSetSw_UpEdge = TRUE;
        }
        stApl[SW_DAY].bSetSwPushOK = FALSE;
    }

	// 밤 스위치 
    if (_SW_SET_LO == SETSW_PUSH) // 스위치를 눌렀을 때 !!!
    {
        if (stApl[SW_NIGHT].SetSwCharterTimer > 50)
        {
            stApl[SW_NIGHT].bSetSwPushOK = TRUE;
        }
    }
    else  // 스위치를 뗐을 때 !
    {
        stApl[SW_NIGHT].SetSwCharterTimer = 0;
        if (stApl[SW_NIGHT].bSetSwPushOK)
        {
            stApl[SW_NIGHT].bSetSw_UpEdge = TRUE;
        }
        stApl[SW_NIGHT].bSetSwPushOK = FALSE;
    }
}



void WriteVal(UINT DutyCycle, UINT SetAVoltage, volatile const UCHAR* DestBuf)
{
    unsigned char SrcBuf[4];

    SrcBuf[0] = (far unsigned char)DutyCycle;
    SrcBuf[1] = (far unsigned char)(DutyCycle >> 8);
    SrcBuf[2] = (far unsigned char)SetAVoltage;
    SrcBuf[3] = (far unsigned char)(SetAVoltage >> 8);

    flash_write((const unsigned char *)SrcBuf, 4, (far unsigned char *)DestBuf);
}



void ReadVal(volatile const UCHAR* SavedBuf, UINT* pSetA_Volt, UINT* pDutyCycle)
{
    unsigned int temp;

    temp = 0x0000;
    temp = SavedBuf[1];
    temp = temp << 8;
    *pDutyCycle = temp | ((unsigned int)SavedBuf[0] & 0x00ff);

    temp = 0x0000;
    temp = SavedBuf[3];
    temp = temp << 8;
    *pSetA_Volt = temp | ((unsigned int)SavedBuf[2] & 0x00ff);
}



void GetAdValue(void)
{
    V_IN_Volt = AdValue[1];
    A_IN_Volt = AdValue[2];
    if (stApl[0].bSetSwPushOK)
    {
        SetA1_Volt = AdValue[3];
    }
    if (stApl[2].bSetSwPushOK)
    {
        SetA3_Volt = AdValue[4];
    }
}



unsigned int GetDutyByCompareCurrent(unsigned int duty, unsigned int setVolt,
                                     unsigned int inVolt, unsigned char CurDayNight)
{
    long double setCurrent; // 변환된 볼륨에의한 셋팅 전류 값
    long double inCurrent;  // 변환된 입력 피드백 전류 값
    long double OffsetDutyCycle;

    if (CurDayNight == DAY) setCurrent = (long double)setVolt * SET_AMP_PER_VOLT1; // 166 x 4 = 664
    else				   setCurrent = (long double)setVolt * SET_AMP_PER_VOLT3; // 380 x 2 = 760

    inCurrent = (((long double)inVolt - 600) / 60) * 1000;  // (635 - 600)/60 * 1000 = 583

    OffsetDutyCycle = ((setCurrent * 6) / 100) + 40; //

    if (setCurrent > inCurrent) // 760 > 583
    {
        if (setCurrent > (inCurrent + OffsetDutyCycle))   // 760 > (583+82)=645
        {
            if (duty < DUTI_MAX)	duty++;
            else				duty = DUTI_MAX;
        }
    }
    else if (setCurrent < inCurrent)
    {
        if ((setCurrent + OffsetDutyCycle) < inCurrent)
        {
            if (duty > 0)		duty--;
        }
    }

    if (AnalogValidTime > 20)
    {
        if (setVolt <= A_SET_V_MIN)
            DutyCycle = 0;
        if (setVolt >= A_SET_V_MAX)
            DutyCycle = DUTI_MAX;
    }

    return duty;
}


void PwOnAplLamp(void)
{
    do
    {
        BefCurDayNight = CurDayNight = GetDayEveningNight(); // NONE, DAY , EVENING , NIGHT 값 저장
        if (CurDayNight == NONE)
            DutyCycle = 0x0;
		else
			DutyCycle = stApl[CurDayNight].DutyCycle; 
		_LAMP_ON = TRUE;
		ChangPwmCycleRegedit(CurDayNight);
        PwmOut(DutyCycle);
		
        CLRWDT();
    }
    while (BeginTimer < 100);
}


ULONG GetSetCurrent(unsigned int set_mV, unsigned char CurDayNight)
{
    ULONG Set_Current;

    Set_Current = ((((ULONG)set_mV) * Multip[CurDayNight]) / 1000);

    return Set_Current;
}





unsigned int GetDutyByCmp(unsigned int duty, unsigned int set_mV,
                                 unsigned char DayNig, unsigned int Offset)
{
	
//	Offset = GetOffSet(stApl[0].Set_Current);	

    if (stApl[DayNig].Set_Current > In_Current) 
    {		
        if (stApl[DayNig].Set_Current > (In_Current + Offset))  
        {
            if (duty < DUTI_MAX)	duty++;
            else					duty = DUTI_MAX;
        }
    }
    else if (stApl[DayNig].Set_Current < In_Current)
    {
        if ((stApl[DayNig].Set_Current + Offset) < In_Current)
        {
            if (duty > 0)		duty--;
        }
    }

    if (AnalogValidTime > 20)
    {
        if (set_mV <= A_SET_V_MIN)
            duty = 0;
        if (set_mV >= A_SET_V_MAX)
            duty = DUTI_MAX;
    }

    return duty;
}





// 셋팅 스위치 눌렀을 때 APL 램프 셋팅 
void OnSetAplLamp(tag_CurDay Sw_DayNig)
{	
	_LAMP_ON = TRUE; // LAMP ON
	if (bCurA_IN_mV_Upd)
	{
		bCurA_IN_mV_Upd = FALSE;		
		In_Current = GetInCurrent(CurA_IN_mV);	// 현재 Setting 및 In 전류 값 가져오기 
		
		DutyCycle = GetDutyByCmp(stApl[Sw_DayNig].DutyCycle, stApl[Sw_DayNig].Setting_mV, Sw_DayNig, 0);
		stApl[Sw_DayNig].DutyCycle = DutyCycle;
//		DutyCycle_Avr = AvrDutyCycle(DutyCycle); // Q?? 
	}
	ChangPwmCycleRegedit(Sw_DayNig);
	PwmOut(DutyCycle);		
}

// 현재(실제) APL LAPM On, Off 처리 
void OnOffAplLamp(tag_CurDay CurDayNig)
{
	if (bBlink_DutyOn && (CurDayNig != NONE)) // Blink Led 가 On 일 때
	{	
		_LED_AplLampState = ON_runled1; // Run 상태 LED On
		_LAMP_ON = TRUE; // LAMP ON
		if (bStEnab)
		{
			bStEnab = FALSE;
			StDelayTimer = 0; 

//			ReadVal((arSavedBuf + (CurDayNig*4)), &stApl[CurDayNig].Setting_mV, &stApl[CurDayNig].DutyCycle);
//			stApl[CurDayNig].Set_Current = GetSetCurrent(stApl[CurDayNig].Setting_mV, CurDayNig);
			DutyCycle = stApl[CurDayNig].DutyCycle;
			ChangPwmCycleRegedit(CurDayNig);
			PwmOut(DutyCycle);	

			if (stApl[CurDayNig].Set_Current > JUNG_GIJUN) StDelayTime = 0;
			else	StDelayTime = 100;	
		}
		else
		{
			if (StDelayTimer >= StDelayTime)
			{
				if (bCurA_IN_mV_Upd)
				{	
					bCurA_IN_mV_Upd = FALSE;
					In_Current = GetInCurrent(CurA_IN_mV);	// 현재 Setting 및 In 전류 값 가져오기 
					
					if (stApl[CurDayNig].Set_Current > JUNG_GIJUN)
						DutyCycle = GetDutyByCmp(DutyCycle, stApl[CurDayNig].Setting_mV, CurDayNig, 0);
					else
						DutyCycle = GetDutyByCmp(DutyCycle, stApl[CurDayNig].Setting_mV, CurDayNig, 100);
				}
				ChangPwmCycleRegedit(CurDayNig);
				PwmOut(DutyCycle);
			}			
		}
		
	}
	else // Blink Led 가 Off 일 때
	{
		_LED_AplLampState = OFF_runled1; // Run 상태 LED Off
		_LAMP_ON = FALSE; // LAMP OFF 
		
		DutyCycle = ((stApl[CurDayNig].DutyCycle * 6) / 100);
		if (DutyCycle >= 20) DutyCycle = DutyCycle + 10;
		else if (DutyCycle >= 10) DutyCycle = DutyCycle - 1;
		else DutyCycle = DutyCycle + 1;
		ChangPwmCycleRegedit(CurDayNig);		
		PwmOut(DutyCycle);	
		bStEnab = TRUE;
		
	}
}

void ChkSwTwoTouch(void)
{	
	// Day
	if (stApl[SW_DAY].bSetSwPushOK)
	{
		if (stApl[SW_DAY].SwPushTimer < 500)
		{
			stApl[SW_DAY].bSwSlightPush = TRUE;
		}
		else
		{
			stApl[SW_DAY].bSwSlightPush = FALSE;
		}
	}
	else
	{
		stApl[SW_DAY].SwPushTimer = 0;
		if (stApl[SW_DAY].bSwSlightPush)
		{
			stApl[SW_DAY].SwTouchCnt++;
		}
		stApl[SW_DAY].bSwSlightPush = FALSE;
	}

	if (stApl[SW_DAY].SwTouchCnt)
	{
		if (stApl[SW_DAY].SwTouchCntTimer < 1000)
		{
			if (stApl[SW_DAY].SwTouchCnt >= 2) // 투 터치 했어 
			{			
				if(CurDayNight == SW_DAY) stApl[SW_DAY].bBlinkEnab = !stApl[SW_DAY].bBlinkEnab; 
				stApl[SW_DAY].SwTouchCnt = 0;
			}
		}
		else
		{
			stApl[SW_DAY].SwTouchCnt = 0;
		}
	}
	else
	{
		stApl[SW_DAY].SwTouchCntTimer = 0;
	}

	// Night 
	if (stApl[SW_NIGHT].bSetSwPushOK)
	{
		if (stApl[SW_NIGHT].SwPushTimer < 500)
		{
			stApl[SW_NIGHT].bSwSlightPush = TRUE;
		}
		else
		{
			stApl[SW_NIGHT].bSwSlightPush = FALSE;
		}
	}
	else
	{
		stApl[SW_NIGHT].SwPushTimer = 0;
		if (stApl[SW_NIGHT].bSwSlightPush)
		{
			stApl[SW_NIGHT].SwTouchCnt++;
		}
		stApl[SW_NIGHT].bSwSlightPush = FALSE;
	}

	if (stApl[SW_NIGHT].SwTouchCnt)
	{
		if (stApl[SW_NIGHT].SwTouchCntTimer < 1000)
		{
			if (stApl[SW_NIGHT].SwTouchCnt >= 2) 
			{			
				if(CurDayNight == SW_NIGHT) stApl[SW_NIGHT].bBlinkEnab = !stApl[SW_NIGHT].bBlinkEnab; 
				stApl[SW_NIGHT].SwTouchCnt = 0;
			}
		}
		else
		{
			stApl[SW_NIGHT].SwTouchCnt = 0;
		}
	}
	else
	{
		stApl[SW_NIGHT].SwTouchCntTimer = 0;
	}	
}


// 기준 셋팅값에 따라 PWM 주기 레지스트 설정 값 변경 
void ChangPwmCycleRegedit(tag_CurDay CurDayNight)
{
	if (stApl[CurDayNight].Set_Current > JUNG_GIJUN)
	{
		if (T2CON != 0x04)
			T2CON = 0x04; // 2000천 간델라 일 떄 !
	}
	else
	{
		if (T2CON != 0x06)
			T2CON = 0x06; // 2000천 간델라 일 떄 !
	}
}

// Tx 에러일 경우 대비, Tx리셋 및 Disable
void Chk232TxErr(void)
{
	if (Com1RxStatus == TX_SET)
	{
		if (Com1SerialTime > 3)
		{
			Com1SerialTime = 0;
			Com1RxStatus = STX_CHK;
			Com1TxCurCnt = 0;
			Com1TxTotalCnt = 0;
			TX1IE = 0;
		}
	}

}

// 셋업 모드에서 셋업 스위치 누르고 뗐을 때 ! 현재 DutyCycle, SetA값 저장 !
void WriteProc(void)
{
	if (stApl[SW_DAY].bSetSw_UpEdge)
	{
		if (stApl[SW_DAY].bWriteEnab)
		{	
			WriteVal(DutyCycle, stApl[SW_DAY].Setting_mV, (arSavedBuf + (SW_DAY * 4)));
			stApl[SW_DAY].bSetSw_UpEdge = FALSE;
			stApl[SW_DAY].bWriteEnab = FALSE;
		}
		else
		{
			stApl[SW_DAY].bSetSw_UpEdge = FALSE;
		}
	}
	
	if (stApl[SW_NIGHT].bSetSw_UpEdge)
	{
		if (stApl[SW_NIGHT].bWriteEnab)
		{			
			WriteVal(DutyCycle, stApl[SW_NIGHT].Setting_mV, (arSavedBuf + (SW_NIGHT * 4)));
			stApl[SW_NIGHT].bSetSw_UpEdge = FALSE;
			stApl[SW_NIGHT].bWriteEnab = FALSE;
		}
		else
		{
			stApl[SW_NIGHT].bSetSw_UpEdge = FALSE;
		}		
	}	

}

ULONG GetInCurrent(ULONG CurA_IN_mV)
{
	ULONG In_Current;
	
	if(CurA_IN_mV >= 600) 
		In_Current = (((ULONG)CurA_IN_mV - 600) * 1000) / 60;  // (630 - 600)/60 * 1000 = 500 mA 
	else 
		In_Current = 0;

	return In_Current;
}

///////////////////////////
//   메인 함수 				//
///////////////////////////

void main(void)
{
    unsigned char i;
	UCHAR ch;

    di();
    Initial();
    InitPort();
    InitTimer0();
    InitAD();
    InitPwm1();
    //UserBaudRate();
    Com1_Init();
    Com2_Init();
    ei();

    DONE = 1;	// A/D Conversion Status bit
    TMR0IE = 1;
    SWDTEN = 1;  // Software Controlled Watchdog Timer Enable bit / 1 = Watchdog Timer is on

	// 낮,밤의 저장된 셍팅전압,전류,듀티값을 얻어온다. 
	for (i=0; i<3; i++)
	{
		ReadVal((arSavedBuf + (i*4)), &stApl[i].Setting_mV, &stApl[i].DutyCycle);
		stApl[i].Set_Current = GetSetCurrent(stApl[i].Setting_mV, i);
	}
    PwOnAplLamp();
    

    //LoadSetupValue();
    modesw = 0xff;
    LedBlinkModeInit();

    MainTimer = 0;
    msec100 = 0;
    Com1SerialTime = 0;
    Com1RxStatus = STX_CHK;
	
	stApl[0].bBlinkEnab = TRUE;	stApl[2].bBlinkEnab = TRUE;	
	
    while (1)
    {
        CLRWDT();
 
		Chk232TxErr();	
		
// BLink 기능	
		// Gps 232Rx 데이타 수신
        if (Com2RxStatus == RX_GOOD) // GPS RX2 통신 GOOD !
        {
            Com2RxStatus = STX_CHK;
            GpsRx2DataProc();
        }
		GpsPPS1Chk(); // GPS Puls 체크

// CDS 낮, 밤 체크 기능 
		// 낮, 밤 체크 
		// 밤 일때 NIG LED ON
        CurDayNight = GetDayEveningNight(); // NONE, DAY , EVENING , NIGHT 값 가져온다. 
        if(CurDayNight == NIGHT) 	_LED_NIG = LED_NIG_ON;
		else						_LED_NIG = LED_NIG_OFF;
		// 낮, 밤이 바뀔 때 처리 
		if (CurDayNight != BefCurDayNight)
		{
			BefCurDayNight = CurDayNight;
			bStEnab = TRUE;
		}		

// CCR 기능 (APL LAMP 출력 제어) 
		// 스위치가 On 및 Edge 여부 체크 
        ChkSetupSw(); 
		// 스위치 Two Touch 여부 체크 
		ChkSwTwoTouch();
		
		//  셋업모드의 PwmDutyCycle, 셋팅값 저장  
		WriteProc();


		// AD 처리 
		bUdtAd = IsUdtAd(arInPut_mV, arIs_AdUpd, AdChSel);
        if(bUdtAd) // input AD 값 얻음.
        {
			// 각 AD 값이 Updated 이면, 각 관련 변수에 저장 한다. 
			GetMyAD();
			
			// 채널 변경 
			if(stApl[SW_DAY].bSetSwPushOK)			AdChSel = ChangeAdChSel(AdChSel, 3);
			else if(stApl[SW_NIGHT].bSetSwPushOK)	AdChSel = ChangeAdChSel(AdChSel, 4);
			else									AdChSel = ChangeAdChSel(AdChSel, 1);	
			Set_AdCh(AdChSel);

			// AD 채널이 변경 되었다. 
			if (AdChSel != BefAdChSel)
			{
				bAdCalcEnable = FALSE;
				AdCalcWaitCnt = 0;
				BefAdChSel = AdChSel;
			}
			
			bAdConversion = FALSE;
			DONE = 1;
        }
		
		// 셋팅모드에서 AMP Lamp 셋업값 얻어온다.
		if ((stApl[SW_DAY].SwPushTimer > 1000) || (stApl[SW_NIGHT].SwPushTimer > 1000))
		{
			bSettingMode = TRUE;	
			
			if (bSetSt)
			{
				_LAMP_ON = TRUE; // LAMP ON
				bSetSt = FALSE;
				DutyCycle = 0;
				SetStTimer = 0;
				T2CON = 0x06; // 2000천 간델라 일 떄 !
				PwmOut(DutyCycle);	
			}
			else
			{
				if(SetStTimer > 1000)
				{
					if(stApl[SW_DAY].bSetSwPushOK)
					{
						stApl[SW_DAY].Set_Current = GetSetCurrent(stApl[SW_DAY].Setting_mV, SW_DAY);
						OnSetAplLamp(SW_DAY);
						stApl[SW_DAY].bWriteEnab = TRUE;
					}
					if(stApl[SW_NIGHT].bSetSwPushOK)
					{
						stApl[SW_NIGHT].Set_Current = GetSetCurrent(stApl[SW_NIGHT].Setting_mV, SW_NIGHT);
						OnSetAplLamp(SW_NIGHT);
						stApl[SW_NIGHT].bWriteEnab = TRUE;
					}					
				}
			}
			bStEnab = TRUE;
		}
		// 일반 모드에서 APL LAMP On, OFF 처리 
		else 		
		{
			if((stApl[0].bWriteEnab == FALSE) && (stApl[2].bWriteEnab == FALSE))
			{
				bSettingMode = FALSE;
				OnOffAplLamp(CurDayNight);
				bSetSt = TRUE;
			}
		}
    }
}



void interrupt isr(void)
{

    if (TMR0IF)
    {
        TMR0IF = 0 ;
        TMR0L = MSEC_L;
        TMR0H = MSEC_H;

        
		if ((CurDayNight == DAY) && (stApl[SW_DAY].bBlinkEnab == FALSE))	bBlink_DutyOn = TRUE;
		else if ((CurDayNight == NIGHT) && (stApl[SW_NIGHT].bBlinkEnab == FALSE))	bBlink_DutyOn = TRUE;
		else bBlink_DutyOn = IsBlink_On();

        Com1SerialTime++;
        Com2SerialTime++;

        if (stApl[0].SetSwCharterTimer < 250)
            stApl[0].SetSwCharterTimer++;
        if (stApl[2].SetSwCharterTimer < 250)
            stApl[2].SetSwCharterTimer++;

        if (BeginTimer < 1000)
            BeginTimer++;

        if (AnalogValidTime < 200)
            AnalogValidTime++;

        if (InDayTimer < 0xff)
            InDayTimer++;
		if (SetStTimer < 0xffff)
            SetStTimer++;
		if (StDelayTimer < 0xffff)
            StDelayTimer++;
		

        msec100++;
        if (msec100 > 100)
        {
            msec100 = 0;
            DayNightTimer++;
            MainTimer++;

            if (SettingReadyTime < 100)	SettingReadyTime++;
            if (NightSetTime < 100)		NightSetTime++;
            if (NightDaySetTime < 100)	NightDaySetTime++;

        }

		if(stApl[SW_DAY].SwPushTimer < 0xffff) stApl[SW_DAY].SwPushTimer++;
		if(stApl[SW_DAY].SwTouchCntTimer < 0xffff) stApl[SW_DAY].SwTouchCntTimer++;
		if(stApl[SW_NIGHT].SwPushTimer < 0xffff) stApl[SW_NIGHT].SwPushTimer++;
		if(stApl[SW_NIGHT].SwTouchCntTimer < 0xffff) stApl[SW_NIGHT].SwTouchCntTimer++;		
    }

    // GPS Rx2 통신 인터럽트
    if ((RC2IE) && (RC2IF))
    {
        RC2IF = 0;
        Com2_Rx();
    }

    if ((TX2IE) && (TX2IF))
    {
        TX2IF = 0;
        Com2_Tx();
    }
    if (OERR2)
    {
        TXEN2 = 0;
        TXEN2 = 1;
        SPEN2 = 0;
        SPEN2 = 1;
        CREN2 = 0;
    }

    if (!CREN2)	CREN2 = 1;


    if ((RC1IE) && (RC1IF))
    {
        RC1IF = 0;
        Com1_Rx();
    }

    if ((TX1IE) && (TX1IF))
    {
        TX1IF = 0;
        Com1_Tx();
    }


    if (OERR1)
    {
        TXEN1 = 0;
        TXEN1 = 1;
        SPEN1 = 0;
        SPEN1 = 1;
        CREN1 = 0;
    }

    if (!CREN1)	CREN1 = 1;


    if (ADIF)
    {
        ADIF = 0;
        if (bAdConversion == FALSE)
        {
            ADBuf = ADRES;
            bAdConversion = TRUE;
        }
        DONE = 0;
    }

}




