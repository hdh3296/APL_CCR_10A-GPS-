

#include <pic18.h>


unsigned int	DutyCycle;



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


void Update_Pwm(void)
{
	DC1B0=(bit)DutyCycle;		//update the PWM duty cycle 
	DC1B1=(bit)(DutyCycle>>1);
	CCPR1L=(DutyCycle>>2);	
}


void Pwm1_Init(void)
{	
	TMR2IE=0;
	TMR2IP=0;
	TMR2IF=0;
	CCP1CON=0x0;	/* select PWM mode */	

	DutyCycle = 0x3ff;
	Update_Pwm();

	PR2=0xff;			
	T2CON=(0x06);		// 16x
}




		





