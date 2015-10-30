
extern	void    Com1_Init(void);
extern	void    Com1_Tx(void);
extern	void    Com1_Rx(void);
extern	void	Com1TxStart(void);








#define	nCOM1_MAX_TX_BUF		100
#define	nCOM1_MAX_RX_BUF		100

extern	unsigned 	char	Com1TxBuffer[nCOM1_MAX_TX_BUF];
extern	unsigned 	char	Com1RxBuffer[nCOM1_MAX_RX_BUF];
extern	unsigned 	char	Com1TxTotalCnt;
extern	unsigned 	char	Com1TxCurCnt;

extern	unsigned 	char   	Com1RxStatus;
extern	unsigned 	char   	Com1RxCurCnt;
extern	unsigned 	char 	Com1SerialTime;
extern	unsigned 	int		Com1BaudRate;

