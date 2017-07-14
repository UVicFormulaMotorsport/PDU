#include <avr/io.h>

#include "pdu.h"

#include "pdu_spi.h"

void
pdu_spi_init(void)
{
	/* SCK, MOSI, SS1, SS2 as outputs */
	DDRB = (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

	/* SS1, SS2 high */
	PORTB |= (1 << PB0) | (1 << PB4);

	/* Enable SPI in master mode */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}
