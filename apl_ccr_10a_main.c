




#include    <pic18.h>

#include	"serial.h"
#include   	"Com1_Port.h"
#include   	"Com2_Port.h"
#include   	"Pwm1.h"
#include   	"analog.h"
#include   	"setup.h"
#include	"apl_ccr_10a_main.h"

extern  void  Timer0Init(void);
extern  void  Initial(void);

#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif

#ifndef	ON_lamp
#define	ON_lamp	1
#endif
#ifndef	OFF_lamp
#define	OFF_lamp	0
#endif

#ifndef	ON_runled1
#define	ON_runled1	0
#endif
#ifndef	OFF_runled1
#define	OFF_runled1	1
#endif



#define		LED_ONOFF_CNT		20

//#define		NIGHT_VOLT			200
//#define		NIGHT_DAY_VOLT		135


unsigned    char  	MainTimer = 0;
unsigned    char	msec100 = 0;


unsigned    char	NightSetTime = 0;
unsigned    char	NightDaySetTime = 0;
unsigned    char	SettingReadyTime = 0;

unsigned    char	DayNightTimer = 0;

unsigned    char	Ghour = 0, Gmin = 0, Gsec = 0;
unsigned    int		Gm1 = 0, Gmsec60000 = 0;


unsigned    char	WakeupTime = 0;
unsigned	char	modesw = 0;
unsigned	int		rx_hour = 0, rx_min = 0, rx_sec = 0;
unsigned	int		OnTime[LED_ONOFF_CNT];
unsigned	int		LedCycle_Msec;
unsigned	int		LedOnDuty_Msec;
unsigned	long	ZeroTimer = 0;
unsigned	long	l_hour, l_min, l_sec;


unsigned    char	NightVolt = 0;
unsigned    char	NightDayVolt = 0;


bit   	FeedbackOn = 0;

bit   	bActiveOn = 0;
bit   	bPwmOn = 0;
bit   	bSearchGps = 0;
bit		bPPS_On = 0;
bit		bPPS_Edge = 0;

bit		bNight = 0;
bit		bNightDay = 0;

bit		bAplLamp_OnEnab = 0;



void    PortInit(void)
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



    _aplLAMP_ON = 0;
    _PWM = 0;
    NoUse_GPS_ON = 1;
    NoUse_FORCE = 0;  // ????
    NoUse_INVALID = 1;
    NoUse_EX2_ON = 0;
    _LED_GpsGoodState = 1;	// 1 : Led Off
    _LED_AplLampState = 1;	// 1 : Led Off
    _LED_RUN2 = 1;			// 1 : Led Off
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
        if (!bPPS_On)
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
        _LED_RUN2 = 1;
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
        if (bNight)	_LED_RUN2 = 0;
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
        if (bNightDay)	_LED_RUN2 = 0;
    }
    else
    {
        NightDaySetTime = 0;
        bNightDay = 0;

        NightSetTime = 0;
        bNight = 0;

        if (SettingReadyTime > 4)
        {
            _LED_RUN2 = !_LED_RUN2;
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
				UpdatePwmDuty();
			}
			else if(AdValue[2] > AdValue[3]){
				if(DutyCycle > 0x0)	DutyCycle--;
				else				DutyCycle=0;

				UpdatePwmDuty();
			}
		}

		if(Gmsec60000 <= OnTime[0]){  		// led on
			_APLLAMP=1;
		}
		else if(Gmsec60000 <= OnTime[1]){  	// led on
			_APLLAMP=0;
		}
		else if(Gmsec60000 <= OnTime[2]){  	// led on
			_APLLAMP=1;
		}
		else if(Gmsec60000 <= OnTime[3]){  	// led on
			_APLLAMP=0;
		}
		else if(Gmsec60000 <= OnTime[4]){  	// led on
			_APLLAMP=1;
		}
		else if(Gmsec60000 <= OnTime[5]){  	// led on
			_APLLAMP=0;
		}
		else if(Gmsec60000 <= OnTime[6]){  	// led on
			_APLLAMP=1;
		}
		else if(Gmsec60000 <= OnTime[7]){  	// led on
			_APLLAMP=0;
		}
		else if(Gmsec60000 <= OnTime[8]){  	// led on
			_APLLAMP=1;
		}
		else if(Gmsec60000 <= OnTime[9]){  	// led on
			_APLLAMP=0;
		}
		else if(Gmsec60000 <= OnTime[10]){  	// led on
			_APLLAMP=1;
		}
		else if(Gmsec60000 <= OnTime[11]){  	// led on
			_APLLAMP=0;
		}
		else if(Gmsec60000 <= OnTime[12]){  	// led on
			_APLLAMP=1;
		}
		else if(Gmsec60000 <= OnTime[13]){  	// led on
			_APLLAMP=0;
		}
		else if(Gmsec60000 <= OnTime[14]){  	// led on
			_APLLAMP=1;
		}
		else if(Gmsec60000 <= OnTime[15]){  	// led on
			_APLLAMP=0;
		}
		else if(Gmsec60000 <= OnTime[16]){  	// led on
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
			UpdatePwmDuty();
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
			_LED_RUN2=1;

			SLEEP();
		}

	}
}

*/

void SetApaLamp(void)
{
	if (bAn2_Updated) // A_IN 아날로그 값이 업데이트 됬으면 ?
	{
		bAn2_Updated = FALSE;
	
		if(bSetSwPushOK1)		SetAVoltage = SetA1_Volt;
		else if(bSetSwPushOK2)	SetAVoltage = SetA3_Volt;
		// Ad2 와 Ad3(또는 Ad4) 값을 비교하여 Pwm 듀티 값을 증가 또는 감소 한다.
		DutyCycle = GetDutyByCompareCurrent(DutyCycle, SetAVoltage, A_IN_Volt, CurDayNight);
							
	}
    _aplLAMP_ON = ON_lamp; // 실제 APL 램프 ON
    UpdatePwmDuty(DutyCycle);
	bPwmOn = TRUE;
}


// APL Lamp 출력 함수이다.
void ApaLampOnOff(void)
{
	// bAplLamp_OnEnab 상태 (-On듀티상태-)에서 
	// _aplLAMP_ON 및 PWM 출력을 내보내면 LAMP에 실제로 불이 켜진다.  
    if (bAplLamp_OnEnab)	
    {
		if (bPwmOn == FALSE)
        	PwmOn();
		
		if (bAgoBlkLedOff)
		{
			bAgoBlkLedOff = FALSE;
			StartTimer = 0;

			if(CurDayNight == DAY) 
				ReadVal(&SavedDutyCycle1, &SavedSetA1_Volt, Saved1Buf, &SetA1_Volt);
			else 
				ReadVal(&SavedDutyCycle3, &SavedSetA3_Volt, Saved3Buf, &SetA3_Volt);			

		}
		else
		{
			if(StartTimer > 100)
			{
				if (bAn2_Updated) // A_IN 아날로그 값이 업데이트 됬으면 ?
				{
					bAn2_Updated = FALSE;

					if(CurDayNight == DAY) 	SetAVoltage = SetA1_Volt;
					else 					SetAVoltage = SetA3_Volt;
					// Ad2 와 Ad3(또는 Ad4) 값을 비교하여 Pwm 듀티 값을 증가 또는 감소 한다.
					DutyCycle = GetDutyByCompareCurrent(DutyCycle, SetAVoltage, A_IN_Volt, CurDayNight);									
				}
			}
		}
        _aplLAMP_ON = ON_lamp; // 실제 APL 램프 ON
        UpdatePwmDuty(DutyCycle);
        bPwmOn = TRUE;
        _LED_AplLampState = ON_runled1; // 상태 LED 깜빡 깜빡 !!!

    }
    else
    {
        if (bPwmOn)
        {
            PwmOff();
            DutyCycle = 0x3ff;
            UpdatePwmDuty(DutyCycle);
        }
        bPwmOn = FALSE;
        _PWM = 0;
        _aplLAMP_ON = OFF_lamp;
        _LED_AplLampState = OFF_runled1;
		bAgoBlkLedOff = TRUE;
    }
}

// GPS 펄스에 의한 엣지가 생기면 !
// msec 값을 999으로 변경하고
// GPS 시,분,초 변수값에 시간을 저장한다.
bit IsAplLamp_On(void)
{
    static bit bAplLamp_On;
	
    if (bPPS_Edge && bPPS_On)
    {
        bPPS_Edge = FALSE;
        if (rx_sec == 59)
        {
            Gm1 = 999;
            Gsec = rx_sec;
            Gmin = rx_min;
            Ghour = rx_hour;
            rx_sec = 0;
        }
    }	

    Gm1++;
    // msec 가 1000(1초)이면 시간 값(초,분,시)을 1씩 증가 시킨다.
    if (Gm1 >= 1000)
    {
        Gm1 = 0;
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

    if (Gm1 == 0)
    {
        ZeroTimer = (unsigned long)((unsigned long)Ghour * (unsigned long)3600000);
        ZeroTimer = ZeroTimer + (unsigned long)((unsigned long)Gmin  * (unsigned long)60000);
        ZeroTimer = ZeroTimer + (unsigned long)((unsigned long)Gsec  * (unsigned long)1000);
    }

    Gmsec60000 = (unsigned int)((ZeroTimer + (unsigned long)Gm1) % (unsigned long)LED_CYCLE_MSEC);

    if (Gmsec60000 < LED_ON_DUTY_MSEC)
    {
        bAplLamp_On = TRUE; // APL LAMP ON
    }
    else
    {
        bAplLamp_On = FALSE;  // APL LAMP off
    }

    return bAplLamp_On;
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
    i = (Gm1 % 100);
    Com1TxBuffer[14] = (unsigned char)((Gm1 / 100) + 0x30);
    Com1TxBuffer[15] = (unsigned char)((i / 10) + 0x30);
    Com1TxBuffer[16] = (unsigned char)((i % 10) + 0x30);

    Com2RxBuffer[18] = 0x0;

    Com1TxCurCnt = 0;
    Com1TxTotalCnt = Com2RxCurCnt;
    Com1TxStart();
}


unsigned char GetDayEveningNight(void)
{
	unsigned char ret;

	if(_CDS_NIGHT_IN) 
		ret = DAY; 
	else	
		ret = NIGHT;

	return ret;
		
}



void ChkSetSw_UpEdge(void)
{	
	if(_SW_SET_HI == SETSW_PUSH){ // 스위치를 눌렀을 때 !!!
		if(SetSwCharterTimer1 > 100){
			bSetSwPushOK1 = TRUE;
		}
	}else{ // 스위치를 뗐을 때 !
		SetSwCharterTimer1 = 0;
		if(bSetSwPushOK1){
			bSetSw_UpEdge1 = TRUE;
		}
		bSetSwPushOK1 = FALSE;
	}

	if(_SW_SET_LO == SETSW_PUSH){ // 스위치를 눌렀을 때 !!!
		if(SetSwCharterTimer2 > 100){
			bSetSwPushOK2 = TRUE;
		}
	}else{ // 스위치를 뗐을 때 !
		SetSwCharterTimer2 = 0;
		if(bSetSwPushOK2){
			bSetSw_UpEdge2 = TRUE;
		}
		bSetSwPushOK2 = FALSE;
	}	
}



void WriteVal(unsigned int DutyCycle, unsigned int SetAVoltage, volatile const unsigned char* DestBuf)
{	
	unsigned char SrcBuf[4];

	SrcBuf[0] = (far unsigned char)DutyCycle;
	SrcBuf[1] = (far unsigned char)(DutyCycle >> 8);
	SrcBuf[2] = (far unsigned char)SetAVoltage;
	SrcBuf[3] = (far unsigned char)(SetAVoltage >> 8);
	
	flash_write((const unsigned char *)SrcBuf, fSIZE, (far unsigned char *)DestBuf);
}


void ReadVal(unsigned int* pSavedDutyCycle, unsigned int* pSavedSetA_Volt, 
			 volatile const unsigned char* SavedBuf, unsigned int* pSetA_Volt)
{
	unsigned int temp;

	temp = 0x0000;
	temp = SavedBuf[1];
	temp = temp << 8;
	*pSavedDutyCycle = temp | ((unsigned int)SavedBuf[0] & 0x00ff);
	DutyCycle = *pSavedDutyCycle;	

	temp = 0x0000;
	temp = SavedBuf[3];
	temp = temp << 8;
	*pSavedSetA_Volt = temp | ((unsigned int)SavedBuf[2] & 0x00ff);
	*pSetA_Volt = *pSavedSetA_Volt; // 주간 셋팅 값 
}


void GetAdValue(void)
{	
	V_IN_Volt = AdValue[1];
	A_IN_Volt = AdValue[2];
	if (bSetSwPushOK1)
	{
		SetA1_Volt = AdValue[3];
	}
	if (bSetSwPushOK2)	
	{
		SetA3_Volt = AdValue[4];
	}
}


#define	A_SET_V_MAX 5000 // mV
#define	A_SET_V_MIN 0
#define A_SET_A_MAX1 10000 // mA
#define A_SET_A_MIN1 0
#define SET_AMP_PER_VOLT1	((A_SET_A_MAX1 - A_SET_A_MIN1) / (A_SET_V_MAX - A_SET_V_MIN)) // 4
#define A_SET_A_MAX2 10000 // mA
#define A_SET_A_MIN2 0
#define SET_AMP_PER_VOLT2	((A_SET_A_MAX2 - A_SET_A_MIN2) / (A_SET_V_MAX - A_SET_V_MIN)) // 4
#define A_SET_A_MAX3 10000 // mA
#define A_SET_A_MIN3 0
#define SET_AMP_PER_VOLT3	((A_SET_A_MAX3 - A_SET_A_MIN3) / (A_SET_V_MAX - A_SET_V_MIN)) // 4
unsigned int GetDutyByCompareCurrent(unsigned int duty, unsigned int setVolt, 
												  unsigned int inVolt, unsigned char CurDayNight)
{
	long double setCurrent; // 변환된 볼륨에의한 셋팅 전류 값
	long double inCurrent;  // 변환된 입력 피드백 전류 값
	long double OffsetDutyCycle;

	if(CurDayNight == DAY) setCurrent = (long double)setVolt * SET_AMP_PER_VOLT1; // 166 x 4 = 664
	else				   setCurrent = (long double)setVolt * SET_AMP_PER_VOLT3; // 380 x 2 = 760
	
	inCurrent = (((long double)inVolt - 600) / 60 ) * 1000; // (635 - 600)/60 * 1000 = 583

	OffsetDutyCycle = ((setCurrent * 6)/100) + 40; // 
	
	if(setCurrent > inCurrent){ // 760 > 583			
		if( setCurrent > (inCurrent + OffsetDutyCycle) ){ // 760 > (583+82)=645
			if(duty < DUTI_MAX)	duty++; 
			else				duty = DUTI_MAX; 
		}
	}else if(setCurrent < inCurrent){
		if( (setCurrent + OffsetDutyCycle) < inCurrent ){
			if(duty > 0)		duty--;
		}
	}		

	if(AnalogValidTime > 20){	
		if(setVolt <= A_SET_V_MIN)
			DutyCycle = 0;
		if(setVolt >= A_SET_V_MAX)
			DutyCycle = DUTI_MAX;
	}
	
	return duty;
}												  


///////////////////////////
//   메인 함수 				//
///////////////////////////

void main(void)
{
	unsigned char i;
	
    di();
    Initial();
    PortInit();
    Timer0Init();
	//UserBaudRate();
    Com1_Init();
    Com2_Init();
    Pwm1_Init();
    AnalogInit();

    _PWM = 0;

    ei();

    //LoadSetupValue();

    modesw = 0xff;
    LedBlinkModeInit();


	do{
		CurDayNight = GetDayEveningNight(); // NONE, DAY , EVENING , NIGHT 값 저장
		if(CurDayNight == DAY) 
			ReadVal(&SavedDutyCycle1, &SavedSetA1_Volt, Saved1Buf, &SetA1_Volt); 
		else
			ReadVal(&SavedDutyCycle3, &SavedSetA3_Volt, Saved3Buf, &SetA3_Volt);

		PwmOn();
		UpdatePwmDuty(DutyCycle);
		_aplLAMP_ON = ON_lamp;
		bPwmOn = TRUE;
		CLRWDT();
	}while(BeginTimer < 100);
	
	
    MainTimer = 0;
    msec100 = 0;
    Com1SerialTime = 0;
    Com1RxStatus = STX_CHK;

	bSetSw_UpEdge1 = FALSE;
	bSetSwPushOK1 = FALSE;

    while (1)
    {
        CLRWDT();
		
		//ReSettingDayNigntChk();
		GpsPPS1Chk(); // GPS Puls 체크 
		ADConversionChk();
		GetAdValue();

		ChkSetSw_UpEdge();

		if (bSetSw_UpEdge1)
		{
			WriteVal(DutyCycle, SetA1_Volt, Saved1Buf);
			bSetSw_UpEdge1 = FALSE;
		}

		if (bSetSw_UpEdge2)
		{
			WriteVal(DutyCycle, SetA3_Volt, Saved3Buf);
			bSetSw_UpEdge2 = FALSE;
		}	 

		if (bSetSwPushOK1 || bSetSwPushOK2)
			SetApaLamp();
		else
        	ApaLampOnOff();

        if (Com2RxStatus == RX_GOOD) // GPS RX2 통신 GOOD !
        {
            Com2RxStatus = STX_CHK;
            GpsRx2DataProc();
        }

        // Tx 에러일 경우 대비, Tx리셋 및 Disable
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
}



void interrupt isr(void)
{

    if (TMR0IF)
    {
        TMR0IF = 0 ;
        TMR0L = MSEC_L;
        TMR0H = MSEC_H;

        bAplLamp_OnEnab = IsAplLamp_On();

        Com1SerialTime++;
        Com2SerialTime++;
		
		if(SetSwCharterTimer1 < 250)	
			SetSwCharterTimer1++;
		if(SetSwCharterTimer2 < 250)	
			SetSwCharterTimer2++;
		
		if(BeginTimer < 1000)
			BeginTimer++;

		if(StartTimer < 1000) 
			StartTimer++;

		if(AnalogValidTime < 200)	
			AnalogValidTime++;

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
        if (!bAdConversion)
        {
            ADBuf = ADRES;
            bAdConversion = TRUE;
        }
        DONE = 0;
    }

}




