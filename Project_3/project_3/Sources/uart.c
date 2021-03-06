//UART0 initialization and addressing
#include "MKL25Z4.h"
#include "UART.h"
#define BAUD 57600

void UART0_init(void){
	uint16_t sbr;
	uint8_t temp;

	//Set clock frequency for PORTA
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Set clock frequency
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);// Select the clock as PLL/2

	PORTA_PCR1 = PORT_PCR_MUX(2);// Set the alternate function for PORTA
	PORTA_PCR2 = PORT_PCR_MUX(2);

	UART0_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); // Switch off UART before setting
	UART0_C1_REG(UART0_BASE_PTR)=0x00; //Set all default configuration


	sbr= (uint16_t)(DEFAULT_SYSTEM_CLOCK/(BAUD *16));

	/* Save off the current value of the uartx_BDH except for the SBR field */
	temp = UART_BDH_REG(UART0_BASE_PTR) & ~(UART_BDH_SBR(0x1F));
	UART_BDH_REG(UART0_BASE_PTR) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));//set SBR field for Baud rate
	UART_BDL_REG(UART0_BASE_PTR) = (uint8_t)(sbr & UART_BDL_SBR_MASK);

	//Set NVIC interrupts and set priority
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn,2);

	UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK; //Setup receiver interrupt

    UART_C2_REG(UART0_BASE_PTR) |= (UART_C2_TE_MASK | UART_C2_RE_MASK ); //Switch on transmitter and receiver
  }


void UART0_IRQHandler(void){
	__asm("cpsid i");
	if((UART0_S1 & UART_S1_TDRE_MASK) && !(Empty(&txbuffer))){
		remove_item(&txbuffer, &UART0_D);

		if(Empty(&txbuffer))
			UART0_C2 &= ~UART_C2_TIE_MASK;	//disable transmitter interrupts if empty
	}

	if((UART0_S1 & UART_S1_RDRF_MASK) && !(Full(&rxbuffer))){
		add_item(&rxbuffer,(uint8_t)UART0_D);

		if(Full(&txbuffer))
			UART0_C2 &= ~UART_C2_RIE_MASK;	//disable receiver interrupts if full
	}
	__asm("cpsie i");
}



























/*#include "MKL25Z4.h"
#include <stdint.h>
#include "uart.h"


void UART0_init(void)
{
	SIM->SCGC4 |= 0x0400;
	SIM->SCGC5 = 0x0200;
	SIM->SOPT2 |= 0x04000000;

	PORTA->PCR[1] = 0x0200;
	PORTA->PCR[2]= 0x0200;

	UART0_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
	UART0_C1_REG(UART0_BASE_PTR)=0x00;

	UART0->BDH = 0x00;
	UART0->BDL = 0x17;

	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn,2);


	UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK;
    UART_C2_REG(UART0_BASE_PTR) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
	UART0->C1 = 0x00;

}


void UART0_IRQHandler(void){
	__asm("cpsid i");
	if((UART0_S1 & UART_S1_TDRE_MASK) && !(Empty(&txbuffer))){
		remove_item(&txbuffer, &UART0_D);

		if(Empty(&txbuffer))
			UART0_C2 &= ~UART_C2_TIE_MASK;
	}

	if((UART0_S1 & UART_S1_RDRF_MASK) && !(Full(&rxbuffer))){
		add_item(&rxbuffer,(uint8_t)UART0_D);

		if(Full(&rxbuffer))
			UART0_C2 &= ~UART_C2_RIE_MASK;
	}
	__asm("cpsie i");
}



void send_char(char c)
	{
	while(!(UART0_S1 & 0x80)) {

	    }
        UART0->D = c;
	}

void send_string(char s[])
{
		int i=0;

		while (s[i] != 0x00)
		{
			send_char(s[i]);
			i++;
		}

}

void delayMs(int n)
{
	int i;
	int j;
	for (i=0;i<n;i++)
	for (j=0;j<n;j++)
	{
	}

}*/


