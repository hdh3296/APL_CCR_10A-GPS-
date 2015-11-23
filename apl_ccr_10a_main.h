

#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif



extern volatile bit RB0x                @ ((unsigned)&PORTB*8)+0;
extern volatile bit RB1x                @ ((unsigned)&PORTB*8)+1;

// I/O Name
#define NoUse_CDS				RA0
#define V_IN 					RA1 
#define _V_IN					V_IN
#define A_IN 					RA2
#define _A_IN					A_IN
#define ISET_HI 				RA3	
#define _VR1_SET_HI				ISET_HI	
#define ISET_LO					RA5
#define _VR2_SET_LO				ISET_LO

#define NIGHT_IN 				RB0x
#define _CDS_NIGHT_IN			NIGHT_IN
#define SAVE_HI 				RB1x
#define _SW_SET_HI				SAVE_HI
#define SAVE_LO 				RB2
#define _SW_SET_LO				SAVE_LO 	
#define NoUse_MODE3 			RB3	
#define NoUse_MODE4 			RB4	
#define NoUse_MODE5 			RB5
#define PGC 					RB6
#define PGD 					RB7
	
#define NoUse_MODE6 			RC0	
#define NoUse_MODE7 			RC1	
#define NoUse_FORCE 			RC2	
#define NoUse_INVALID 			RC3	
#define NoUse_RC4	 			RC4	
#define LED_RUN2	 			RC5	
#define _LED_NIG	 			LED_RUN2 // Night 상태 LED 
#define NoUse_TX1	 			RC6	// CCP3/CK1/TX1/CANTX	
#define NoUse_RX1	 			RC7	// CANRX/RX1/DT1/CCP4

#define LED_ON 					RD0
#define _LAMP_ON				LED_ON // APL Lamp On/Off  		
#define NoUse_EX2_ON 			RD1	
#define NoUse_GPS_ON 			RD2	
#define LED_RUN0 				RD3
#define _LED_AplLampState		LED_RUN0 // APL Lamp On 듀티 LED
#define PWM 					RD4
#define	_PWM					PWM	
#define LED_RUN1 				RD5
#define	_LED_GpsGoodState		LED_RUN1 // GPS RX2 수신시, 'A' 데이타 수신 상태 LED	 
#define NoUse_TX2 				RD6	// 통신 TX2 
#define TX_DP 					RD7	// 통신 RX2
#define	_RX_CDS					TX_DP

#define PPS 					RE0
#define	_PPS					PPS 
#define NoUse_OPTION 			RE1	
#define NoUse_M3 				RE2
#define MCLR					RE3

// Tris
#define NoUse_CDS_TRIS 			TRISA0	
#define V_IN_TRIS 				TRISA1	
#define A_IN_TRIS 				TRISA2	
#define ISET_HI_TRIS 			TRISA3	
#define ISET_LO_TRIS			TRISA5	

#define NIGHT_IN_TRIS 			TRISB0	
#define SAVE_HI_TRIS 			TRISB1	
#define SAVE_LO_TRIS 			TRISB2	
#define NoUse_MODE3_TRIS 		TRISB3	
#define NoUse_MODE4_TRIS 		TRISB4	
#define NoUse_MODE5_TRIS 		TRISB5
#define PGC_TRIS 				TRISB6
#define PGD_TRIS 				TRISB7
	
#define NoUse_MODE6_TRIS 		TRISC0	
#define NoUse_MODE7_TRIS 		TRISC1	
#define NoUse_FORCE_TRIS 		TRISC2	
#define NoUse_INVALID_TRIS 		TRISC3	
#define NoUse_RC4_TRIS	 		TRISC4	
#define LED_RUN2_TRIS			TRISC5	
#define NoUse_TX1_TRIS	 		TRISC6	
#define NoUse_RX1_TRIS	 		TRISC7	

#define LED_ON_TRIS 			TRISD0	
#define NoUse_EX2_ON_TRIS 		TRISD1	
#define NoUse_GPS_ON_TRIS 		TRISD2	
#define LED_RUN0_TRIS 			TRISD3	
#define PWM_TRIS 				TRISD4	
#define LED_RUN1_TRIS 			TRISD5	
#define NoUse_TX2_TRIS 			TRISD6	
#define TX_DP_TRIS 				TRISD7	

#define PPS_TRIS 				TRISE0	
#define NoUse_OPTION_TRIS 		TRISE1	
#define NoUse_M3_TRIS 			TRISE2	





#define     MSEC_H    0xfc
#define     MSEC_L    0x18

// LED 깜빡이는 1싸이클에 대하여 ON 듀티 시간(msec) 값을 구한다.
#define		COUNT_MIN	60      	// 1분당  LED ON 횟수
#define		LED_ON_DUTY_RATIO	50	// LED ON 듀티 비(%)
#define		LED_CYCLE_MSEC		(60000 / (COUNT_MIN))
// Lamp Blink에서의 On 주기 시간(ms) 
#define		LED_ON_DUTY_MSEC	((ULONG)((ULONG)(((ULONG)LED_CYCLE_MSEC) * ((ULONG)LED_ON_DUTY_RATIO)) / 100))

#define SETSW_PUSH		0 // 스위치 눌렀을 때가 0 값이다.

#define	chV_IN	1
#define	chA_IN	2
#define	chVR1	3
#define	chVR2	4


#define LED_NIG_ON	0
#define LED_NIG_OFF	1


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

#define JUNG_GIJUN 2000


#define		LED_ONOFF_CNT		20

//#define		NIGHT_VOLT			200
//#define		NIGHT_DAY_VOLT		135









unsigned    char  	MainTimer = 0;
unsigned    char	msec100 = 0;


unsigned    char	NightSetTime = 0;
unsigned    char	NightDaySetTime = 0;
unsigned    char	SettingReadyTime = 0;

unsigned    char	DayNightTimer = 0;

unsigned    char	Ghour = 0, Gmin = 0, Gsec = 0; // Gps 로 부터 수신된 RxData 값 
unsigned    int		Gms1 = 0; // 마이컴의 인터럽트에 의한 발생된 msec 값 저장 변수 
unsigned    int		Gms60000 = 0; // 현재 시간값을 ms단위로 환산한 값 저장 변수 


unsigned    char	WakeupTime = 0;
unsigned	char	modesw = 0;
unsigned	int		rx_hour = 0, rx_min = 0, rx_sec = 0;
unsigned	int		OnTime[LED_ONOFF_CNT];
unsigned	int		LedCycle_Msec;
unsigned	int		LedOnDuty_Msec;
unsigned	long	ZeroTimer = 0;
unsigned	long	l_hour, l_min, l_sec;
unsigned int 	SetDutyCycle = 0;


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

bit		bBlink_DutyOn = 0;


unsigned int BeginTimer = 0;








unsigned    char    SetSwCharterTimer1 = 0;
unsigned    char    SetSwCharterTimer2 = 0;

unsigned char InDayTimer = 0;











// 5000이면 5V이다.
unsigned int SetAVoltage=0; 
unsigned int SetA1_Volt=0; // SER A1 Voltage, AN0 , 낮 
unsigned int SetA3_Volt=0; // SER A3 Voltage, AN2 , 밤 
unsigned int A_IN_Volt=0; // A_IN Voltage, AN3
unsigned int V_IN_Volt=0; // V_IN Voltage, AN4

bit bAgoBlkLedOff;
unsigned int StartTimer = 0;
unsigned int AnalogValidTime = 0;


#define WRSIZE	12
volatile const unsigned char  arSavedBuf[WRSIZE] = {0, };






unsigned int SetStTimer = 0;
unsigned int DaySwPushTimer = 0;
unsigned int DaySwTouchCntTimer = 0;

bit bDaySwSlightPush = FALSE;
volatile unsigned char DaySwTouchCnt = 0;
bit bDayBlinkEnab;
bit bDayWriteEnab = FALSE;

extern  void  Initial(void);
extern  void  InitTimer0(void);
extern void    InitPort(void);
extern void    UserBaudRate(void);
extern void  Serial2Check(void);
extern void GpsPPS1Chk(void);
extern void LedBlinkModeInit(void);
extern unsigned int ReSettingDayNigntChk(void);
extern void ApaLampOnOff(void);
extern bit IsBlink_On(void);
extern void GpsRx2DataProc(void);
extern void ChkSetupSw(void);
extern unsigned char GetDayEveningNight(void);
extern void WriteVal(unsigned int DutyCycle, unsigned int SetAVoltage, volatile const unsigned char* DestBuf);
extern void GetAdValue(void);
extern unsigned int GetDutyByCompareCurrent(unsigned int duty, unsigned int setVolt, 
												  unsigned int inVolt, unsigned char CurDayNight);
extern void SetApaLamp(void);
extern void PwOnAplLamp(void);


extern bit IsInLED_ON(unsigned char bLedState, unsigned char* Timer);

extern unsigned int GetDutyByCmp(unsigned int duty, unsigned int set_mV,
                                     unsigned int Out_mV, unsigned char CurDayNight);


