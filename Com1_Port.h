
extern	void    Com1_Init(void);
extern	void    Com1_Tx(void);
extern	void    Com1_Rx(void);
extern	void	Com1TxStart(void);








#define	COM1_MAX_TX_BUF		100
#define	COM1_MAX_RX_BUF		100

extern	unsigned 	char	Com1TxBuffer[COM1_MAX_TX_BUF];
extern	unsigned 	char	Com1RxBuffer[COM1_MAX_RX_BUF];
extern	unsigned 	char	Com1TxCnt;
extern	unsigned 	char	Com1TxThisPt;

extern	unsigned 	char   	Com1RxStatus;
extern	unsigned 	char   	Com1RxCurCnt;
extern	unsigned 	char 	Com1SerialTime;
extern	unsigned 	int		Com1BaudRate;

