/*
 * init.c
 *
 *  Created on: 10 дек. 2016 г.
 *      Author: werwolf
 */

#include <msp430.h>

void init_mcu(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

// -----========== Basic Clock System ==========-----

    /*
     * Basic Clock System Control 2
     *
     * SELM_0 -- DCOCLK
     * DIVM_0 -- Divide by 1
     * ~SELS -- DCOCLK
     * DIVS_0 -- Divide by 1
     * ~DCOR -- DCO uses internal resistor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;

    if (CALBC1_1MHZ != 0xFF)
    {
    	/* Follow recommended flow. First, clear all DCOx and MODx bits. Then
    	 * apply new RSELx values. Finally, apply new DCOx and MODx bit values.
    	 */
	    DCOCTL = 0x00;
	    BCSCTL1 = CALBC1_1MHZ;      /* Set DCO to 1MHz */
	    DCOCTL = CALDCO_1MHZ;
	}

	/*
	 * Basic Clock System Control 1
	 *
	 * XT2OFF -- Disable XT2CLK
	 * ~XTS -- Low Frequency
	 * DIVA_0 -- Divide by 1
	 *
	 * Note: ~XTS indicates that XTS has value zero
	 */
	BCSCTL1 |= XT2OFF | DIVA_0;

	/*
	 * Basic Clock System Control 3
	 *
	 * XT2S_0 -- 0.4 - 1 MHz
	 * LFXT1S_2 -- If XTS = 0, XT1 = VLOCLK ; If XTS = 1, XT1 = 3 - 16-MHz crystal or resonator
	 * XCAP_1 -- ~6 pF
	 */
	BCSCTL3 = XT2S_0 | LFXT1S_2 | XCAP_1;

// -----========== GPIO ==========-----

    /* Port 1 Port Select 2 Register */
    P1SEL2 = BIT1 | BIT2 | BIT5 | BIT6 | BIT7;
    /* Port 1 Output Register */
    P1OUT = 0;
    /* Port 1 Port Select Register */
    P1SEL = BIT1 | BIT2 | BIT5 | BIT6 | BIT7;
    /* Port 1 Direction Register */
    P1DIR = BIT2 | BIT5 | BIT7;
    /* Port 1 Interrupt Edge Select Register */
    P1IES = BIT3 | BIT4;
    /* Port 1 Interrupt Flag Register */
    P1IFG = 0;
    /* Port 1 Interrupt Enable Register */
    P1IE = BIT3 | BIT4;

    /* Port 2 Output Register */
    P2OUT = BIT1 | BIT7;
    /* Port 2 Port Select Register */
    P2SEL = 0;
    /* Port 2 Direction Register */
	P2DIR = BIT0 | BIT1 | BIT2 | BIT7;
    /* Port 2 Resistor Enable Register */
    P2REN = 0;
    /* Port 2 Interrupt Edge Select Register */
    P2IES = 0;
    /* Port 2 Interrupt Flag Register */
    P2IFG = 0;

// -----========== ADC ==========-----

    /* disable ADC10 during initialization */
    ADC10CTL0 &= ~ENC;

    /*
     * Control Register 0
     *
     * ~ADC10SC -- No conversion
     * ~ENC -- Disable ADC
     * ~ADC10IFG -- Clear ADC interrupt flag
     * ADC10IE -- Enable ADC interrupt
     * ADC10ON -- Switch On ADC10
     * REFON -- Enable ADC reference generator
     * ~REF2_5V -- Set reference voltage generator = 1.5V
     * ~MSC -- Disable multiple sample and conversion
     * ~REFBURST -- Reference buffer on continuously
     * ~REFOUT -- Reference output off
     * ~ADC10SR -- Reference buffer supports up to ~200 ksps
     * ADC10SHT_3 -- 64 x ADC10CLKs
     * SREF_1 -- VR+ = VREF+ and VR- = VSS
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL0 = ADC10IE | ADC10ON | REFON | ADC10SHT_3 | SREF_1;

    /*
     * Control Register 1
     *
     * ~ADC10BUSY -- No operation is active
     * CONSEQ_0 -- Single channel single conversion
     * ADC10SSEL_0 -- ADC10OSC
     * ADC10DIV_3 -- Divide by 4
     * ~ISSH -- Input signal not inverted
     * ~ADC10DF -- ADC10 Data Format as binary
     * SHS_0 -- ADC10SC
     * INCH_10 -- Temperature Sensor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL1 = CONSEQ_0 | ADC10SSEL_0 | ADC10DIV_3 | SHS_0 | INCH_10;


    /* Software delay for REFON to settle */
    __delay_cycles(30000);

    /* enable ADC10 */
    ADC10CTL0 |= ENC;

// -----========== UART ==========-----

    /* Disable USCI */
    UCA0CTL1 |= UCSWRST;

    /*
     * Control Register 1
     *
     * UCSSEL_2 -- SMCLK
     * ~UCRXEIE -- Erroneous characters rejected and UCAxRXIFG is not set
     * ~UCBRKIE -- Received break characters do not set UCAxRXIFG
     * ~UCDORM -- Not dormant. All received characters will set UCAxRXIFG
     * ~UCTXADDR -- Next frame transmitted is data
     * ~UCTXBRK -- Next frame transmitted is not a break
     * UCSWRST -- Enabled. USCI logic held in reset state
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    UCA0CTL1 = UCSSEL_2 | UCSWRST;

    /*
     * Modulation Control Register
     *
     * UCBRF_0 -- First stage 0
     * UCBRS_1 -- Second stage 1
     * ~UCOS16 -- Disabled
     *
     * Note: ~UCOS16 indicates that UCOS16 has value zero
     */
    UCA0MCTL = UCBRF_0 | UCBRS_1;

    /* Baud rate control register 0 */
    UCA0BR0 = 104;

    /* Enable USCI */
    UCA0CTL1 &= ~UCSWRST;

// -----========== SPI ==========-----

    /* Disable USCI */
    UCB0CTL1 |= UCSWRST;

    /*
     * Control Register 0
     *
     * ~UCCKPH -- Data is changed on the first UCLK edge and captured on the following edge
     * ~UCCKPL -- Inactive state is low
     * UCMSB -- MSB first
     * ~UC7BIT -- 8-bit
     * UCMST -- Master mode
     * UCMODE_0 -- 3-Pin SPI
     * UCSYNC -- Synchronous Mode
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    //UCB0CTL0 = UCMSB | UCMST | UCMODE_0 | UCSYNC;
    UCB0CTL0 =  UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;

    /*
     * Control Register 1
     *
     * UCSSEL_2 -- SMCLK
     * UCSWRST -- Enabled. USCI logic held in reset state
     */
    UCB0CTL1 = UCSSEL_2 | UCSWRST;

    /* Bit Rate Control Register 0 */
    UCB0BR0 = 10;

    /* Enable USCI */
    UCB0CTL1 &= ~UCSWRST;

// -----========== Timer ==========-----

    /*
     * TA0CCTL0, Capture/Compare Control Register 0
     *
     * CM_0 -- No Capture
     * CCIS_0 -- CCIxA
     * ~SCS -- Asynchronous Capture
     * ~SCCI -- Latched capture signal (read)
     * ~CAP -- Compare mode
     * OUTMOD_0 -- PWM output mode: 0 - OUT bit value
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    TA0CCTL0 = CM_0 | CCIS_0 | OUTMOD_0 | CCIE;

    /* TA0CCR0, Timer_A Capture/Compare Register 0 */
    //TA0CCR0 = 59999;
    TA0CCR0 = 23999;
    /*
     * TA0CTL, Timer_A3 Control Register
     *
     * TASSEL_1 -- ACLK
     * ID_1 -- Divider - /2
     * MC_1 -- Up Mode
     */
    TA0CTL = TASSEL_1 | ID_1 | MC_1;

}
