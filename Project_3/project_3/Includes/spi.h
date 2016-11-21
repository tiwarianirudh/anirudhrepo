#ifndef SPI
#define SPI

#include "MKL25Z4.h"

void SPI_init(void);
uint8_t spi_datatransfer(uint8_t spi_data);
void cs_low(void);
void cs_high(void);

#endif
