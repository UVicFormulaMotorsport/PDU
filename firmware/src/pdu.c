/*
 * PDUv17.c
 *
 * Created: 2017-03-30 10:02:18 AM
 * Author : Darryl Ring
 */ 

 
#include <avr/interrupt.h>
#include <avr/io.h>

#include "pdu.h"

#include "pdu_can.h"
#include "pdu_spi.h"


volatile uint8_t outputs_20a = DEFAULT_20A_OUTPUTS;
volatile uint8_t outputs_5a_a = DEFAULT_5A_OUTPUTS_A;
volatile uint8_t outputs_5a_b = DEFAULT_5A_OUTPUTS_B;

uint8_t outputs_5a_changed = 0;


int main(void)
{
	/* Initialize peripherals */
	pdu_spi_init();
	
	/* 20 A switch outputs */
	DDRA = 0xFF;

	/* Configure 5 A switch outputs */

	PORTB &= ~(1 << PB4); // CS2 low
	SPDR = 0x40; // Write
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00; // IODIRA
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00; // All outputs
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1 << PB4); // CS2 high
	
	PORTB &= ~(1 << PB4); // CS2 low
	SPDR = 0x40; // Write
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x01; // IODIRB
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00; // All outputs
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1 << PB4); // CS2 high

	PORTB &= ~(1 << PB4); // CS2 low
	SPDR = 0x40; // Write
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x12; // GPIOA
	while(!(SPSR & (1<<SPIF)));
	SPDR = outputs_5a_a;
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1 << PB4); // CS2 high
	
	PORTB &= ~(1 << PB4); // CS2 low
	SPDR = 0x40; // Write
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x13; // GPIOB
	while(!(SPSR & (1<<SPIF)));
	SPDR = outputs_5a_b;
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1 << PB4); // CS2 high

	pdu_can_init();

	/* Enable interrupts */
	sei();

	while (1) {
		PORTA = outputs_20a;

		if (outputs_5a_changed) {
			PORTB &= ~(1 << PB4); // CS2 low
			SPDR = 0x40; // Write
			while(!(SPSR & (1<<SPIF)));
			SPDR = 0x12; // GPIOA
			while(!(SPSR & (1<<SPIF)));
			SPDR = outputs_5a_a;
			while(!(SPSR & (1<<SPIF)));
			PORTB |= (1 << PB4); // CS2 high
			
			PORTB &= ~(1 << PB4); // CS2 low
			SPDR = 0x40; // Write
			while(!(SPSR & (1<<SPIF)));
			SPDR = 0x13; // GPIOB
			while(!(SPSR & (1<<SPIF)));
			SPDR = outputs_5a_b;
			while(!(SPSR & (1<<SPIF)));
			PORTB |= (1 << PB4); // CS2 high

			outputs_5a_changed = 0;
		}
	}
}


/*
20 A channels
Port A (7: Ch6, 6: Ch5, 5: Ch8, 4: Ch7, 3: Ch2, 2: Ch1, 1: Ch4, 0: Ch3)
*/

void
pdu_output_20a_enable(uint8_t output)
{
	switch (output) {
	case 1:
		outputs_20a |= (1 << 2);
		break;
	case 2:
		outputs_20a |= (1 << 3);
		break;
	case 3:
		outputs_20a |= (1 << 0);
		break;
	case 4:
		outputs_20a |= (1 << 1);
		break;
	case 5:
		outputs_20a |= (1 << 6);
		break;
	case 6:
		outputs_20a |= (1 << 7);
		break;
	case 7:
		outputs_20a |= (1 << 4);
		break;
	case 8:
		outputs_20a |= (1 << 5);
		break;
	}
}

void
pdu_output_20a_disable(uint8_t output)
{
	switch (output) {
	case 1:
		outputs_20a &= ~(1 << 2);
		break;
	case 2:
		outputs_20a &= ~(1 << 3);
		break;
	case 3:
		outputs_20a &= ~(1 << 0);
		break;
	case 4:
		outputs_20a &= ~(1 << 1);
		break;
	case 5:
		outputs_20a &= ~(1 << 6);
		break;
	case 6:
		outputs_20a &= ~(1 << 7);
		break;
	case 7:
		outputs_20a &= ~(1 << 4);
		break;
	case 8:
		outputs_20a &= ~(1 << 5);
		break;
	}
}


/*
GPA (7: Ch5, 6: Ch6, 5: Ch7, 4: Ch8, 3: Ch4, 2: Ch3, 1: Ch2, 0: Ch1)
GPB (7: Ch16, 6: Ch15, 5: Ch14, 4: Ch13, 3: Ch9, 2: Ch10, 1: Ch11, 0: Ch12)
*/

void
pdu_output_5a_enable(uint8_t output)
{
	uint8_t outputs_5a_a_new = outputs_5a_a;
	uint8_t outputs_5a_b_new = outputs_5a_b;
	
	switch (output) {
		case 1:
			outputs_5a_a_new |= 0x01;
			break;
		case 2:
			outputs_5a_a_new |= 0x02;
			break;
		case 3:
			outputs_5a_a_new |= 0x04;
			break;
		case 4:
			outputs_5a_a_new |= 0x08;
			break;
		case 5:
			outputs_5a_a_new |= 0x80;
			break;
		case 6:
			outputs_5a_a_new |= 0x40;
			break;
		case 7:
			outputs_5a_a_new |= 0x20;
			break;
		case 8:
			outputs_5a_a_new |= 0x10;
			break;
		case 9:
			outputs_5a_b_new |= 0x08;
			break;
		case 10:
			outputs_5a_b_new |= 0x04;
			break;
		case 11:
			outputs_5a_b_new |= 0x02;
			break;
		case 12:
			outputs_5a_b_new |= 0x01;
			break;
		case 13:
			outputs_5a_b_new |= 0x10;
			break;
		case 14:
			outputs_5a_b_new |= 0x20;
			break;
		case 15:
			outputs_5a_b_new |= 0x40;
			break;
		case 16:
			outputs_5a_b_new |= 0x80;
			break;
	}
	
	if (outputs_5a_a_new != outputs_5a_a) {
		outputs_5a_a = outputs_5a_a_new;
		outputs_5a_changed = 1;
	}
	
	if (outputs_5a_b_new != outputs_5a_b) {
		outputs_5a_b = outputs_5a_b_new;
		outputs_5a_changed = 1;
	}
}

void
pdu_output_5a_disable(uint8_t output)
{
	uint8_t outputs_5a_a_new = outputs_5a_a;
	uint8_t outputs_5a_b_new = outputs_5a_b;
	
	switch (output) {
		case 1:
			outputs_5a_a_new &= ~0x01;
			break;
		case 2:
			outputs_5a_a_new &= ~0x02;
			break;
		case 3:
			outputs_5a_a_new &= ~0x04;
			break;
		case 4:
			outputs_5a_a_new &= ~0x08;
			break;
		case 5:
			outputs_5a_a_new &= ~0x80;
			break;
		case 6:
			outputs_5a_a_new &= ~0x40;
			break;
		case 7:
			outputs_5a_a_new &= ~0x20;
			break;
		case 8:
			outputs_5a_a_new &= ~0x10;
			break;
		case 9:
			outputs_5a_b_new &= ~0x08;
			break;
		case 10:
			outputs_5a_b_new &= ~0x04;
			break;
		case 11:
			outputs_5a_b_new &= ~0x02;
			break;
		case 12:
			outputs_5a_b_new &= ~0x01;
			break;
		case 13:
			outputs_5a_b_new &= ~0x10;
			break;
		case 14:
			outputs_5a_b_new &= ~0x20;
			break;
		case 15:
			outputs_5a_b_new &= ~0x40;
			break;
		case 16:
			outputs_5a_b_new &= ~0x80;
			break;
	}
	
	if (outputs_5a_a_new != outputs_5a_a) {
		outputs_5a_a = outputs_5a_a_new;
		outputs_5a_changed = 1;
	}
	
	if (outputs_5a_b_new != outputs_5a_b) {
		outputs_5a_b = outputs_5a_b_new;
		outputs_5a_changed = 1;
	}
}
