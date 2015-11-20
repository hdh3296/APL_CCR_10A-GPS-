

#include <pic18.h>
#include "Pwm1.h"


// PWM Mode On
void PwmOn(void)
{
	CCP1M0=1;
	CCP1M1=1;
	CCP1M2=1;
	CCP1M3=1;
}

void PwmOff(void)
{
	CCP1M0=0;
	CCP1M1=0;
	CCP1M2=0;
	CCP1M3=0;
}


void UpdatePwmDuty(unsigned int DutyCycle)
{
	DC1B0 = (bit)DutyCycle; 	//update the PWM duty cycle 
	DC1B1 = (bit)(DutyCycle>>1);
	CCPR1L = (DutyCycle>>2);	//13.04.30
}



void Pwm1_Init(void)
{	
	TMR2IE=0;
	TMR2IP=0;
	TMR2IF=0;
	CCP1CON=0x0;	/* select PWM mode */	

	DutyCycle = DUTI_MAX; // Q???
	UpdatePwmDuty(DutyCycle); // Q???

	PR2=0xff;			
	T2CON=(0x06);		// 16x
}




		





