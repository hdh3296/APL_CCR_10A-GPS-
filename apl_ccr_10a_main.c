




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

#ifndef	ON_APLLAMP
#define	ON_APLLAMP	1
#endif
#ifndef	OFF_APLLAMP
#define	OFF_APLLAMP	0
#endif

#ifndef	ON_RUNLED1
#define	ON_RUNLED1	0
#endif
#ifndef	OFF_RUNLED1
#define	OFF_RUNLED1	1
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

bit		bAplLamp_On = 0;


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



    _LAMP_ON = 0;
    _PWM = 0;
    NoUse_GPS_ON = 1;
    NoUse_FORCE = 0;  // ??????
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
// ??? 1초마다 High 값이 수신된다.
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

	if(An0_Update){
		An0_Update=0;
		if(WakeupTime < 200) WakeupTime++;
	}
}



void MyApplication(void)
{
	ActiveOnChk();
	if(bActiveOn || !NoUse_MODE5){
		if(!bPwmOn)	PwmOn();

		if(An2_Update && An3_Update){
			An2_Update=0;
 			An3_Update=0;

			if(AdValue[2] < AdValue[3]){
				if(dutycycle < 0x3ff)	dutycycle++;
				else					dutycycle=0x3ff;
				Update_Pwm();
			}
			else if(AdValue[2] > AdValue[3]){
				if(dutycycle > 0x0)	dutycycle--;
				else				dutycycle=0;

				Update_Pwm();
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
			dutycycle=0x3ff;
			Update_Pwm();
		}
		bPwmOn=0;

		PWM=0;
		_APLLAMP=0;
		GPS_ON=1;

		_LED_AplLampState=1;

		if(WakeupTime > 0){
			An0_Update=0;
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


// APL Lamp 출력 함수이다.
void MyApplication(void)
{
	// bAplLamp_On 상태에서 PWM 출력을 내보내면 LAMP에 실제로 불이 켜진다.  
    if (bAplLamp_On)	
    {
        if (!bPwmOn)
            PwmOn();

        if (An2_Update && An3_Update)
        {
            An2_Update = 0;
            An3_Update = 0;

            if (AdValue[2] < AdValue[3])
            {
                if (dutycycle < 0x3ff)	dutycycle++;
                else					dutycycle = 0x3ff;
                Update_Pwm();
            }
            else if (AdValue[2] > AdValue[3])
            {
                if (dutycycle > 0x0)	dutycycle--;
                else				dutycycle = 0;

                Update_Pwm();
            }
        }

        _LAMP_ON = ON_APLLAMP;
        bPwmOn = TRUE;
        _LED_AplLampState = ON_RUNLED1;

    }
    else
    {
        if (bPwmOn)
        {
            PwmOff();
            dutycycle = 0x3ff;
            Update_Pwm();
        }
        bPwmOn = FALSE;
        _PWM = 0;
        _LAMP_ON = OFF_APLLAMP;
        _LED_AplLampState = OFF_RUNLED1;
    }
}

// GPS 펄스에 의한 엣지가 생기면 !
// msec 값을 999으로 변경하고
// GPS 시,분,초 변수값에 시간을 저장한다.
bit IsAplLamp_On()
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

    Gmsec60000 = (unsigned int)((ZeroTimer + (unsigned long)Gm1) % (unsigned long)LedCycle_Msec);

    if (Gmsec60000 < LedOnDuty_Msec)
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
void GpsRx2DataProc()
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


void main(void)
{
    di();
    Initial();
    PortInit();
    Timer0Init();
//	UserBaudRate();
    Com1_Init();
    Com2_Init();
    Pwm1_Init();
    AnalogInit();

    _PWM = 0;

    ei();

    LoadSetupValue();

    modesw = 0xff;
    LedBlinkModeInit();

    MainTimer = 0;
    msec100 = 0;
    Com1SerialTime = 0;
    Com1RxStatus = STX_CHK;
    dutycycle = 0x1ff;
    Update_Pwm();

// LED 깜빡임 1싸이클에 대하여 ON  듀티 시간(msec)을 구한다.
#define		COUNT_MIN	20      	// 1분당  LED ON 횟수
#define		LED_ON_DUTY_RATIO	50	// LED ON 듀티 비(%)

    LedCycle_Msec  = (60000 / COUNT_MIN);
    LedOnDuty_Msec = ((LedCycle_Msec * LED_ON_DUTY_RATIO) / 100);


    while (1)
    {
        CLRWDT();

//		ReSettingDayNigntChk();
//		ModeChk();

        GpsPPS1Chk(); // GPS Puls 체크 
        ADConversionChk();
        MyApplication();

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

        bAplLamp_On = IsAplLamp_On();

        Com1SerialTime++;
        Com2SerialTime++;

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
            bAdConversion = 1;
        }
        DONE = 0;
    }

}




