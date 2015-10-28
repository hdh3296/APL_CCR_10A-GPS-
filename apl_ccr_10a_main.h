

extern volatile bit RB0x                @ ((unsigned)&PORTB*8)+0;
extern volatile bit RB1x                @ ((unsigned)&PORTB*8)+1;



#define CDS 		RA0	
#define V_MON 		RA1	
#define I_MON 		RA2	
#define I_SET 		RA3	
#define RA5_NC		RA5	

#define MODE0 		RB0x	
#define MODE1 		RB1x	
#define MODE2 		RB2	
#define MODE3 		RB3	
#define MODE4 		RB4	
#define MODE5 		RB5
#define PGC 		RB6
#define PGD 		RB7

	
#define MODE6 		RC0	
#define MODE7 		RC1	
#define FORCE 		RC2	
#define INVALID 	RC3	
#define RC4_NC	 	RC4	
#define RUNLED2	 	RC5	
#define TX1	 		RC6	
#define RX1	 		RC7	


#define LED_ON 		RD0	
#define EX2_ON 		RD1	
#define GPS_ON 		RD2	
#define RUNLED0 	RD3	
#define PWM 		RD4	
#define RUNLED1 	RD5	
#define TX2 		RD6	
#define RX2 		RD7	


#define PPS 		RE0	
#define OPTION 		RE1	
#define M3 			RE2	





#define CDS_TRIS 		TRISA0	
#define V_MON_TRIS 		TRISA1	
#define I_MON_TRIS 		TRISA2	
#define I_SET_TRIS 		TRISA3	
#define RA5_NC_TRIS		TRISA5	

#define MODE0_TRIS 		TRISB0	
#define MODE1_TRIS 		TRISB1	
#define MODE2_TRIS 		TRISB2	
#define MODE3_TRIS 		TRISB3	
#define MODE4_TRIS 		TRISB4	
#define MODE5_TRIS 		TRISB5
#define PGC_TRIS 		TRISB6
#define PGD_TRIS 		TRISB7

	
#define MODE6_TRIS 		TRISC0	
#define MODE7_TRIS 		TRISC1	
#define FORCE_TRIS 		TRISC2	
#define INVALID_TRIS 	TRISC3	
#define RC4_NC_TRIS	 	TRISC4	
#define RUNLED2_TRIS	TRISC5	
#define TX1_TRIS	 	TRISC6	
#define RX1_TRIS	 	TRISC7	


#define LED_ON_TRIS 	TRISD0	
#define EX2_ON_TRIS 	TRISD1	
#define GPS_ON_TRIS 	TRISD2	
#define RUNLED0_TRIS 	TRISD3	
#define PWM_TRIS 		TRISD4	
#define RUNLED1_TRIS 	TRISD5	
#define TX2_TRIS 		TRISD6	
#define RX2_TRIS 		TRISD7	


#define PPS_TRIS 		TRISE0	
#define OPTION_TRIS 	TRISE1	
#define M3_TRIS 		TRISE2	



/*
#define RXLED       	LATC4 	//0
#define TXLED       	LATD3	//0
#define TX_EN       	LATD2	//0
*/

/*
#define RUNLED0_TRIS 	TRISD3	//0
#define RUNLED1_TRIS 	TRISD5	//0
#define RUNLED2_TRIS 	TRISD6	//0


#define MODE0_TRIS 		TRISB0	//0
#define MODE1_TRIS 		TRISB1	//0
#define MODE2_TRIS 		TRISB2	//0
#define MODE3_TRIS 		TRISB3	//0
#define MODE4_TRIS 		TRISB4	//0
#define MODE5_TRIS 		TRISB5	//0
#define MODE6_TRIS 		TRISC0	//0
#define MODE7_TRIS 		TRISC1	//0
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





