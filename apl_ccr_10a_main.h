

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
#define _SW1_SET_HI				SAVE_HI
#define SAVE_LO 				RB2
#define _SW2_SET_LO				SAVE_LO 	
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
#define _LED_RUN2	 			LED_RUN2
#define NoUse_TX1	 			RC6	// CCP3/CK1/TX1/CANTX	
#define NoUse_RX1	 			RC7	// CANRX/RX1/DT1/CCP4

#define LED_ON 					RD0
#define _LAMP_ON				LED_ON 		
#define NoUse_EX2_ON 			RD1	
#define NoUse_GPS_ON 			RD2	
#define LED_RUN0 				RD3
#define _LED_GpsGoodState		LED_RUN0 
#define PWM 					RD4
#define	_PWM					PWM	
#define LED_RUN1 				RD5
#define	_LED_AplLampState		LED_RUN1 
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



/*
#define RXLED       	LATC4 	//0
#define TXLED       	LATD3	//0
#define TX_EN       	LATD2	//0
*/

/*
#define LED_RUN0_TRIS 	TRISD3	//0
#define LED_RUN1_TRIS 	TRISD5	//0
#define LED_RUN2_TRIS 	TRISD6	//0


#define NIGHT_IN_TRIS 		TRISB0	//0
#define SAVE_HI_TRIS 		TRISB1	//0
#define SAVE_LO_TRIS 		TRISB2	//0
#define NoUse_MODE3_TRIS 		TRISB3	//0
#define NoUse_MODE4_TRIS 		TRISB4	//0
#define NoUse_MODE5_TRIS 		TRISB5	//0
#define NoUse_MODE6_TRIS 		TRISC0	//0
#define NoUse_MODE7_TRIS 		TRISC1	//0
*/


/*
#define	CPU45K80		1
#define	U_COMPILER		1
*/




/*
#define	MAX_ELEV            4
#define	HOST_DATA_RECOD     42
#define	MAX_SAVE_BUF_CAN    HOST_DATA_RECOD * MAX_ELEV
*/


#define     MSEC_H    0xfc
#define     MSEC_L    0x18





