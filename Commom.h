
typedef	unsigned char	UCHAR;
typedef unsigned int	UINT;
typedef unsigned long 	ULONG;

#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif

#define SET_DAY		0
#define SET_NIGHT	2

#define	A_SET_V_MAX 5000 // mV
#define	A_SET_V_MIN 0
#define A_SET_A_MAX1 10000 // mA
#define A_SET_A_MIN1 0
#define SET_AMP_PER_VOLT1	(((ULONG)(A_SET_A_MAX1 - A_SET_A_MIN1) * (ULONG)1000) / (ULONG)(A_SET_V_MAX - A_SET_V_MIN)) // 4
#define A_SET_A_MAX2 10000 // mA
#define A_SET_A_MIN2 0
#define SET_AMP_PER_VOLT2	(((ULONG)(A_SET_A_MAX2 - A_SET_A_MIN2) * (ULONG)1000) / (ULONG)(A_SET_V_MAX - A_SET_V_MIN)) // 4
#define A_SET_A_MAX3 2000 // mA
#define A_SET_A_MIN3 0
#define SET_AMP_PER_VOLT3	(((ULONG)(A_SET_A_MAX3 - A_SET_A_MIN3) * (ULONG)1000) / (ULONG)(A_SET_V_MAX - A_SET_V_MIN)) // 4
ULONG Multip[] = {(SET_AMP_PER_VOLT1), (SET_AMP_PER_VOLT2), (SET_AMP_PER_VOLT3)}; 



typedef enum{DAY = 0, EVENING = 1, NIGHT = 2, NONE = 100} tag_CurDay;
tag_CurDay	CurDayNight;

bit bSetSwPushOK_Day = FALSE;
bit bSetSw_UpEdge_Day = FALSE;
bit bSetSwPushOK_Night = FALSE;
bit bSetSw_UpEdge_Night = FALSE;

UCHAR nADSUM = 0;

typedef struct 
{
	unsigned int SetA;
}tag_Apl;
tag_Apl		stApl[3] = {
					{0,},	// Set0
					{0,},	// Set1
					{0,},	// Set2
			};




unsigned int CurA_IN_mV = 0;
unsigned int CurV_IN = 0;
bit bCurA_IN_mVUpd = FALSE;

ULONG Set_Current; // 변환된 볼륨에의한 셋팅 전류 값
ULONG In_Current;  // 변환된 입력 피드백 전류 값


bit bSetSt= FALSE;
bit bStEnab = FALSE;

extern void ReadVal(volatile const UCHAR* SavedBuf, UINT* pSetA_Volt, UINT* pDutyCycle);
extern ULONG GetSetCurrent(unsigned int set_mV, unsigned char CurDayNight);
extern void SetAplLamp(tag_CurDay CurDayNight);