
#include    <pic18.h>

#include    "Com2_Port.h"
#include    "serial.h"
#include    "MyUtil.h"


unsigned char	Com2TxBuffer[COM2_MAX_TX_BUF];
unsigned char	Com2RxBuffer[COM2_MAX_RX_BUF];
unsigned char	Com2TxCnt=0;
unsigned char	Com2TxThisPt=0;
unsigned char   Com2RxStatus=0;
unsigned char	Com2RxCurCnt=0;
unsigned char	Com2SerialTime=0x0;
unsigned int	Com2BaudRate=9600;

unsigned char	Com2Xor=0;




void    Com2_Init(void)
{
	unsigned int tmpbaudrate;

	SPBRG2 = DIVIDER;     	

	tmpbaudrate=((int)(FOSC/(16UL * Com2BaudRate) -1));
	SPBRG2 = tmpbaudrate;     	

	TXSTA2 = (SPEED|NINE_BITS|0x20);
	RCSTA2 = (NINE_BITS|0x90);
	TRISD6=OUTPUT;
	TRISD7=INPUT;

    RC2IE=1;	    // USART RX interrupt enable
    TX2IE=1;	    // USART TX interrupt enable

    RC2IF=0;	    // USART RX interrupt enable
    TX2IF=0;	    // USART TX interrupt enable

	UART2MD=0;


/*
	SPBRG1 = DIVIDER;     	

	tmpbaudrate=((int)(FOSC/(16UL * Com1BaudRate) -1));
	SPBRG1 = tmpbaudrate;     	

	TXSTA1 = (SPEED|NINE_BITS|0x20);
	RCSTA1 = (NINE_BITS|0x90);
	TRISC6=OUTPUT;
	TRISC7=INPUT;

    RC1IE=1;	    // USART RX interrupt enable
    TX1IE=1;	    // USART TX interrupt enable

    RC1IF=0;	    // USART RX interrupt enable
    TX1IF=0;	    // USART TX interrupt enable

	UART1MD=0;

////////////////////////////	UART2MD=1;  // ??????
*/

}



void    Com2TxStart(void)
{
   	Com2RxStatus=TX_SET;   
    TXREG2=Com2TxBuffer[Com2TxThisPt];
	Com2TxThisPt++;
	TX2IE=1;
}




void Com2_Tx(void)
{					
	if((Com2TxThisPt >= Com2TxCnt)){
		Com2SerialTime=0;
		Com2TxThisPt=0;
 		Com2TxCnt=0;	
		TX2IE=0;
	}
	else{
		
		if(Com2TxThisPt >= COM2_MAX_TX_BUF){
			Com2SerialTime=0;
			Com2TxThisPt=0;
	 		Com2TxCnt=0;	
			TX2IE=0;
		}
		else{
			TXREG2=Com2TxBuffer[Com2TxThisPt];
			Com2TxThisPt++;
			Com2SerialTime=0;
			Com2RxStatus = TX_SET;
		}
	}
}


//$GPRMC,114455.532,A,3735.0079,N,12701.6446,E,0.000000,121.61,110706,,*0A

unsigned int XorCal(unsigned char xor)
{
	Com2Xor=(Com2Xor ^ xor);
	return(0);	
}


// GPS Rx2 통신 함수 
void Com2_Rx(void)
{
   	unsigned char   buf;
   	unsigned char   temp;


   	buf = RCREG2;

    if(Com2RxStatus != TX_SET){   

		if(Com2SerialTime > 4){
            Com2RxCurCnt=0;
		}

        Com2SerialTime=0;

        Com2RxBuffer[Com2RxCurCnt]=buf;

        if(Com2RxCurCnt < (COM2_MAX_RX_BUF-1)){
            Com2RxCurCnt++;
        }
        else{
            Com2RxCurCnt=0;
            Com2RxBuffer[Com2RxCurCnt]=buf;
        }


        switch(Com2RxStatus){
        	case    STX_CHK:
                if( (Com2RxCurCnt == 1) && (buf == '$')){
					Com2Xor=(buf);
                }
                else if( (Com2RxCurCnt == 2) && (buf == 'G')){
					Com2Xor=(buf);
//					XorCal(buf);
                }
                else if( (Com2RxCurCnt == 3) && (buf == 'P')){
					XorCal(buf);
                }
                else if( (Com2RxCurCnt == 4) && (buf == 'R')){
					XorCal(buf);
                }
                else if( (Com2RxCurCnt == 5) && (buf == 'M')){
					XorCal(buf);
                }
                else if( (Com2RxCurCnt == 6) && (buf == 'C')){
					XorCal(buf);
                }
                else if( (Com2RxCurCnt == 7) && (buf == ',')){
					XorCal(buf);
					Com2RxStatus=DATA_CHK;
                }
				else{
					Com2RxCurCnt=0;
				}

        		break;
        	case    DATA_CHK:                                    
//				XorCal(buf);
	            if( buf == '*'){					
                    Com2RxStatus=CHKSUM_HIGH_CHK;
                }
				else{
					XorCal(buf);
				}
                break;
        	case    CHKSUM_LOW_CHK:
        		temp=(Com2Xor & 0x0f);
				if(temp > 9)	temp=(temp + '7');	
				else			temp=(temp + '0');

        		if(temp==buf){
                    Com2RxStatus=COMMAND_CHK;
        		}
        		else    Com2RxStatus=RX_ERROR;

        		break;
        	case    CHKSUM_HIGH_CHK:
        		temp=(Com2Xor >> 4);
				if(temp > 9)	temp=(temp + '7');	
				else			temp=(temp + '0');

        		if(temp==buf){
                    Com2RxStatus=CHKSUM_LOW_CHK;
        		}
        		else    Com2RxStatus=RX_ERROR;

        		break;
        	case    COMMAND_CHK:
				if(buf == 0x0a){
                    Com2RxStatus=RX_GOOD;
				}
				break;
        	case    RX_GOOD:
				break;
        	case    RX_ERROR:
        		Com2RxStatus=STX_CHK;
        		Com2RxCurCnt=0;
        		break;
        	default:
        		Com2RxStatus=STX_CHK;
        		Com2RxCurCnt=0;
        		break;
        }
    }

}
