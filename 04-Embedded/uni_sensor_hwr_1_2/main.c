#include <msp430.h> 
#include <uni_sensor.h>
#include <hal.h>
#include <xbee.h>

//====================================================================
#define STATE_ENERGYSAVING			0x01
#define STATE_ACTIVE				0x02

#define DS_TAG_ADC10_1				0x10DA
#define DS_CAL_ADC_GAIN_FACTOR		0x0002
#define DS_CAL_ADC_OFFSET			0x0004
#define DS_CAL_ADC_15VREF_FACTOR	0x0006
#define DS_CAL_ADC_15T30			0x0008
#define DS_CAL_ADC_15T85			0x000A
//====================================================================
unsigned char OwnAddress, RFType = 0, DoorIntDir[2] = {1, 1};
int n_izm, TX_index, RX_index, TX_length, TXRequest = 0;
int datanum, ADC_res, window_size = WINDOW_SIZE;
int FrameStartFlag = 0, FrameLength, SensorState = STATE_ENERGYSAVING;
unsigned int *ptrCal30, *ptrCal85, Cal30, Cal85;
float param[4] = {0.0, 0.0, 0.0, 0.0}, Tconst;
unsigned char TX_buffer[TX_BUFFER_SIZE], RX_buffer[RX_BUFFER_SIZE];
float tmp1[WINDOW_SIZE], tmp2[WINDOW_SIZE];

unsigned char tx_stat = 0x00;

void main(void)
{
	init_mcu();

	P2OUT &= ~BIT0; // get RF module out of RESET
	__delay_cycles(10000);
	P2OUT |= BIT0; // get RF module out of RESET
	__delay_cycles(300);
	P2OUT &= ~BIT2;
	__delay_cycles(3000);
	//P2OUT |= BIT2;

	// Calculate Temp sensor calibrating constant
	ptrCal30 = (unsigned int *) (DS_TAG_ADC10_1 + DS_CAL_ADC_15T30);
	Cal30 = *ptrCal30;
	ptrCal85 = (unsigned int *) (DS_TAG_ADC10_1 + DS_CAL_ADC_15T85);
	Cal85 = *ptrCal85;
	Tconst = (55 / (float)(Cal85 - Cal30));

	IE2 = UCA0RXIE | UCA0TXIE;  // Enable interrupts
	__bis_SR_register(GIE);

	ZigbeeModuleInit();

	while(1)
	{
		//__delay_cycles(200);  // delay 3 cycles
		if(TXRequest == 1)
		{
			if(RFType == RFMODULETYPE_XBEE)
			{
				XBEE_senddata_api();
			}
			else
			{
				if(RFType == RFMODULETYPE_MRF24)
				{
					tx_stat = MRF_sendPacket(0);
				}
			}
			datanum = 0;
			TXRequest = 0;
		}
		if(SensorState == STATE_ENERGYSAVING)
		{
			//__bis_SR_register(LPM3_bits); // go to LPM3
		}
	}
}

/*
 *  ======== ADC10 Interrupt Service Routine ========
 */
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{
	int i;
	float Vadc;
	if(n_izm==1)  //Temperature measurement done
	{
	  ADC_res = ADC10MEM;
	  //param[0] = (float)(ADC_res - (int)Cal30) * Tconst + 30.0;
	  tmp1[datanum] = (float)(ADC_res - (int)Cal30) * Tconst + 30.0;
	  ADC10CTL0 &= ~ENC;	// disable ADC
	  ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_11; // set channel 11 (1/2 voltage)
	  ADC10CTL0 |= ENC;		// enable ADC
	  P2OUT &= ~BIT7;		// close RR divider to GND
	  n_izm = 2;
	  ADC10CTL0 |= ADC10SC;
	}
	else  // Vcc measurement done
	{
	  ADC_res = ADC10MEM;
	  Vadc = (float)((float)ADC_res / 1023) * 1.5;
	  //param[3] = Vadc * 2;
	  tmp2[datanum] = Vadc * 2;
	  datanum++;
	  P2OUT |= BIT7;		// open RR divider chain to reduce current leakage
	  if(datanum == WINDOW_SIZE)
	  {
		param[0] = 0;
		param[3] = 0;
		for(i = 0; i < WINDOW_SIZE; i++)
		{
			param[0] += (tmp1[i]/(float)WINDOW_SIZE);
			param[3] += (tmp2[i]/(float)WINDOW_SIZE);
		}
		TXRequest = 1;
	  }
	}
}

/*
 *  ======== USCI A0/B0 TX Interrupt Handler Generation ========
 *
 * Here are several important notes on using USCI_A0/B0 TX interrupt Handler:
 * 1. User could use the following code as a template to service the interrupt
 *    handler. Just simply copy and paste it into your user definable code
 *    section.
 *  For UART and SPI configuration:
    if (IFG2 & UCA0TXIFG) {
    }
    else if (IFG2 & UCB0TXIFG) {
    }
 *  For I2C configuration:
    if (IFG2 & UCA0/B0TXIFG) {
    }
    else if (IFG2 & UCA0/B0RXIFG) {
    }
 * 2. User could also exit from low power mode and continue with main
 *    program execution by using the following instruction before exiting
 *    this interrupt handler.
 *
 *    __bic_SR_register_on_exit(LPMx_bits);
 */
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR_HOOK(void)
{
	if (IFG2 & UCA0TXIFG)		// UART TX Interrupt
	{
		if(TX_index < TX_length)
		{
			UCA0TXBUF = TX_buffer[TX_index];
			TX_index++;
		}
		else
		{
			__delay_cycles(150000);
			P2OUT |= BIT2; 						// Xbee module request sleep mode
			IFG2 &= ~UCA0TXIFG;		// Reset interrupt flag
		}
	}
	/*else if (IFG2 & UCB0TXIFG)	// SPI TX Interrupt
	{
		if(TX_index < TX_length)
		{
			UCB0TXBUF = TX_buffer[TX_index];
			TX_index++;
		}
		else
		{
			_delay_cycles(2000);
			//SPI_CS_control(1);
		}
		IFG2 &= ~UCB0TXIFG;		// Reset interrupt flag
	}*/
}

/*
 *  ======== USCI A0/B0 RX Interrupt Handler Generation ========
 */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void)
{
	if (IFG2 & UCA0RXIFG)			// UART RX interrupt
	{
		if(UCA0RXBUF == 0x7E && FrameStartFlag == 0)
		{
			FrameStartFlag = 1;
			RX_index = 0;
		}
		if(FrameStartFlag == 1)
		{
			if(RX_index == 2)
			{
				FrameLength = (RX_buffer[1] * 256 + RX_buffer[2]);
				if(FrameLength == 0)
				{
					FrameStartFlag = 0;
				}
			}
			RX_buffer[RX_index] = UCA0RXBUF;
			if(RX_index == FrameLength + 3)
			{
				//Verify frame checksum
			}
			RX_index++;
			if(RX_index > RX_BUFFER_SIZE - 1) RX_index = (RX_BUFFER_SIZE - 1);
		}
		/*else
		{
			RX_buffer[RX_index] = UCA0RXBUF;
			RX_index++;
			if(RX_index > RX_BUFFER_SIZE - 1) RX_index = (RX_BUFFER_SIZE - 1);
		}*/
		// Clear interrupt flag
		IFG2 &= ~UCA0RXIFG;
	}
	/*else if (IFG2 & UCB0RXIFG)		// SPI RX interrupt
	{
		if(RX_index < TX_length)
		{
			RX_buffer[RX_index] = UCB0RXBUF;
			RX_index++;
			if(RX_index > RX_BUFFER_SIZE - 1) RX_index = (RX_BUFFER_SIZE - 1);
		}
		else
		{
			_delay_cycles(20);
			//SPI_CS_control(1);
		}
		IFG2 &= ~UCB0RXIFG;			// Reset interrupt flag;
	}*/
}

/*
 *  ======== Timer0_A3 Interrupt Service Routine ========
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR_HOOK(void)
{
	n_izm = 1;
	ADC10CTL0 &= ~ENC;
	ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_10;
	ADC10CTL0 |= ENC;
	ADC10CTL0 |= ADC10SC;
}

/*
 *  ======== Port 1 Interrupt Service Routine ========
 */
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR_HOOK(void)
{
	__bic_SR_register(GIE);
	if((P1IFG & BIT3) == BIT3)
	{
		__delay_cycles(2000);
		if((P1IN & BIT3) == 0 && DoorIntDir[0] == 0)	// work if P1IES = 0;
		{
			TA0R = 0x00;
			param[1] = 0.0;
			P1IES |= BIT3;
			DoorIntDir[0] = 1;
			__bis_SR_register(GIE);
			TXRequest = 1;
		}
		if((P1IN & BIT3) == BIT3 && DoorIntDir[0] == 1)	// work if P1IES = 1;
		{
			TA0R = 0x00;
			param[1] = 1.0;
			P1IES &= ~BIT3;
			DoorIntDir[0] = 0;
			__bis_SR_register(GIE);
			TXRequest = 1;
		}
		P1IFG &= ~BIT3; // reset Interrupt flag
	}
	if((P1IFG & BIT4) == BIT4)
	{
		__delay_cycles(2000);
		if((P1IN & BIT4) == 0 && DoorIntDir[1] == 0)	// work if P1IES = 0;
		{
			TA0R = 0x00;
			param[2] = 0.0;
			P1IES |= BIT4;
			DoorIntDir[1] = 1;
			__bis_SR_register(GIE);
			TXRequest = 1;
		}
		if((P1IN & BIT4) == BIT4 && DoorIntDir[1] == 1)	// work if P1IES = 1;
		{
			TA0R = 0x00;
			param[2] = 1.0;
			P1IES &= ~BIT4;
			DoorIntDir[1] = 0;
			__bis_SR_register(GIE);
			TXRequest = 1;
		}
		P1IFG &= ~BIT4; // reset Interrupt flag
	}
}
