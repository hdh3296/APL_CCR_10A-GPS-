
extern	void    Com2_Init(void);
extern	void    Com2_Tx(void);
extern	void    Com2_Rx(void);
extern	void	Com2TxStart(void);








#define	COM2_MAX_TX_BUF		100
#define	COM2_MAX_RX_BUF		100

extern	unsigned 	char	Com2TxBuffer[COM2_MAX_TX_BUF];
extern	unsigned 	char	Com2RxBuffer[COM2_MAX_RX_BUF];
extern	unsigned 	char	Com2TxCnt;
extern	unsigned 	char	Com2TxThisPt;

extern	unsigned 	char   	Com2RxStatus;
extern	unsigned 	char   	Com2RxCurCnt;
extern	unsigned 	char 	Com2SerialTime;
extern	unsigned 	int		Com2BaudRate;

