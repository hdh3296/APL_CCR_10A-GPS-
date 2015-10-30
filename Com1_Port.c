

#include    <pic18.h>

#include    "Com1_Port.h"
#include    "serial.h"
#include    "MyUtil.h"


unsigned char	Com1TxBuffer[nCOM1_MAX_TX_BUF];
unsigned char	Com1RxBuffer[nCOM1_MAX_RX_BUF];
unsigned char	Com1TxTotalCnt = 0;
unsigned char	Com1TxCurCnt = 0;
unsigned char   Com1RxStatus = 0;
unsigned char	Com1RxCurCnt = 0;
unsigned char	Com1SerialTime = 0x0;
unsigned int	Com1BaudRate = 19200;



void    Com1_Init(void)
{
    unsigned int tmpbaudrate;

    SPBRG1 = DIVIDER;

    tmpbaudrate = ((int)(FOSC / (16UL * Com1BaudRate) - 1));
    SPBRG1 = tmpbaudrate;

    TXSTA1 = (SPEED | NINE_BITS | 0x20);
    RCSTA1 = (NINE_BITS | 0x90);
    TRISC6 = OUTPUT;
    TRISC7 = INPUT;

    RC1IE = 1;	  // USART RX interrupt enable
    TX1IE = 1;	  // USART TX interrupt enable

    RC1IF = 0;	  // USART RX interrupt enable
    TX1IF = 0;	  // USART TX interrupt enable

    UART1MD = 0;

////////////////////////////	UART2MD=1;  // ??????
}


	
void    Com1TxStart(void)
{
    Com1RxStatus = TX_SET;
    TXREG1 = Com1TxBuffer[Com1TxCurCnt];
    Com1TxCurCnt++;
    TX1IE = 1;
}


void Com1_Tx(void)
{
    if ((Com1TxCurCnt < Com1TxTotalCnt))
    {

        if (Com1TxCurCnt < nCOM1_MAX_TX_BUF)
        {
            TXREG1 = Com1TxBuffer[Com1TxCurCnt];
            Com1TxCurCnt++;
            Com1SerialTime = 0;
            Com1RxStatus = TX_SET;
        }
        else
        {			
            Com1SerialTime = 0;
            Com1TxCurCnt = 0;
            Com1TxTotalCnt = 0;
            TX1IE = 0;
        }
    }	
    else 
    {
        Com1SerialTime = 0;
        Com1TxCurCnt = 0;
        Com1TxTotalCnt = 0;
        TX1IE = 0;
    }
}




void Com1_Rx(void)
{
    unsigned char   buf;
    unsigned char   temp;


    buf = RCREG1;

    if (Com1RxStatus != TX_SET)
    {

        if (Com1SerialTime > 4)
        {
            Com1RxCurCnt = 0;
        }

        Com1SerialTime = 0;

        Com1RxBuffer[Com1RxCurCnt] = buf;

        if (Com1RxCurCnt < (nCOM1_MAX_RX_BUF - 1))
        {
            Com1RxCurCnt++;
        }
        else
        {
            Com1RxCurCnt = 0;
            Com1RxBuffer[Com1RxCurCnt] = buf;
        }

        switch (Com1RxStatus)
        {
        case    STX_CHK:
            if (Com1RxCurCnt == 1)
            {
                Com1RxBuffer[0] = buf;
                Crc = 0xffff;
                Crc_Calulate((unsigned int)buf);
            }
            else if (Com1RxCurCnt == 2)
            {
                Crc_Calulate((unsigned int)buf);
            }
            else if (Com1RxCurCnt == 3)
            {
                Com1RxStatus = DATA_CHK;
                Crc_Calulate((unsigned int)buf);
            }
            else
            {
                Com1RxCurCnt = 1;
                Com1RxBuffer[0] = buf;
                Crc = 0xffff;
                Crc_Calulate((unsigned int)buf);
            }
            break;
        case    DATA_CHK:
            if (Com1RxCurCnt == (Com1RxBuffer[3] + 4))
            {
                Com1RxStatus = CHKSUM_LOW_CHK;
            }
            Crc_Calulate((unsigned int)buf);
            break;
        case    CHKSUM_LOW_CHK:
            temp = (unsigned char)(Crc & 0x00ff);
            if (temp == buf)
            {
                Com1RxStatus = CHKSUM_HIGH_CHK;
            }
            else    Com1RxStatus = RX_ERROR;
            break;
        case    CHKSUM_HIGH_CHK:
            Crc = ((Crc >> 8) & 0x00ff);
            temp = (unsigned char)Crc;
            if (buf == temp)
            {
                Com1RxStatus = RX_GOOD;
            }
            else    Com1RxStatus = RX_ERROR;
            break;
        case    RX_GOOD:
            break;
        case    RX_ERROR:
            Com1RxStatus = STX_CHK;
            Com1RxCurCnt = 0;
            break;
        default:
            Com1RxStatus = STX_CHK;
            Com1RxCurCnt = 0;
            break;
        }
    }

}








