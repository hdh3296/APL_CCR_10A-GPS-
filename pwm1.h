

#define DUTI_MAX 0x3ff // 1023


unsigned int	DutyCycle;


extern	void InitPwm1(void);
extern  void PwmOut(unsigned int DutyCycle);
extern	void PwmOn(void);
extern	void PwmOff(void);
