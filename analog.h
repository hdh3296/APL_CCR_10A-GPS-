
extern	void			AnalogInit(void);
extern	unsigned char	ADConversionChk(void);


#define	AD_SUMCNT		10
#define	nAD_MAX_CHANEL	7

#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif


bit	bAn0_Updated = 0;
bit	bAn1_Updated = 0;
bit	bAn2_Updated = 0;
bit	bAn3_Updated = 0;
bit	bAn4_Updated = 0;
bit	bAn5_Updated = 0;
bit	bAn6_Updated = 0;
bit	bAn7_Updated = 0;

bit	bAdConversion = 0;

unsigned	long    SumAD	= 0;
unsigned 	long    InPutAD = 0;
unsigned 	int	    ADBuf 	= 0;
unsigned	int	    SumCnt	= 0;
unsigned    int   	AdChSel	= 0;

unsigned    int     AdValue[nAD_MAX_CHANEL]; // 채널별 Ad 값 저장 버퍼 








