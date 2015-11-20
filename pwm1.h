

#define DUTI_MAX 0x3ff // 1023


unsigned int	DutyCycle;


extern	void Pwm1_Init(void);
extern  void UpdatePwmDuty(unsigned int DutyCycle);
extern	void PwmOn(void);
extern	void PwmOff(void);
