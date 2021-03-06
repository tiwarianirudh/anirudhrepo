#include "SPI.h"

void SPI_init(void)
{

	//Turning ON clock for the C module
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;

	//Enable Clock for SPI0
	SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;

	PORTC_PCR4 = PORT_PCR_MUX(0x01);
	GPIOC_PDDR = (1<<4);				//PortC PIN4 as output
	GPIOC_PSOR = (1<<4);				//SS high initially
	PORTC_PCR5 = PORT_PCR_MUX(0x02);    //PTC5 to mux 2 [SCK]
	PORTC_PCR6 = PORT_PCR_MUX(0x02);    //PTC6 to mux 2 [MOSI]
	PORTC_PCR7 = PORT_PCR_MUX(0x02);    //PTC7 to mux 2 [MISO]

	SPI0_C1 = SPI_C1_MSTR_MASK;		//Enabling SPI as master

	SPI0_BR = (SPI_BR_SPPR(0x02) | SPI_BR_SPR(0x01)); //Setting Bitrate 1Mbps

	SPI0_C1 |= SPI_C1_SPE_MASK;    //SPI0 Enable
}

uint8_t spi_datatransfer(uint8_t spi_data)
{
	while(!(SPI_S_SPTEF_MASK & SPI0_S));
	SPI0_D = spi_data;    //Write char to SPI
	while(!(SPI_S_SPRF_MASK & SPI0_S));
	return SPI0_D;
}

void cs_low(void)
{
	GPIOC_PCOR = (1<<4);
}

void cs_high(void)
{
	GPIOC_PSOR = (1<<4);
}


