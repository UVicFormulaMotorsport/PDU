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

volatile uint32_t pdu_clock = 0;

volatile uint8_t pdu_can_trigger = 0;
volatile uint8_t pdu_fuse_trigger = 0;
volatile uint8_t pdu_input_trigger = 0;

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
		PORTA = outputs_20a;	// Set 20A pins to default setting as defined in pdu.h

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

void
pdu_timer_init(void)
{
	/* Output compare interrupts every 1 ms */
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A   = 999;

	/* Set timer clock prescaler to 8 for a 1 MHz timer clock */
	TCCR1B |= (1 << CS11);
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
		outputs_20a |= (1 << 2); //Pin PA2 or SW 20A R1
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
		outputs_20a |= (1 << 6); //Pin PA6 or SW 20A R6
		break;
	case 6:
		outputs_20a |= (1 << 7); //Pin PA7 or SW 20A R7
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

uint8_t
pdu_channel_enabled(uint8_t output){
	

		switch (output) {
		case 1:
			return ((outputs_5a_a & 0x01) > 0);
			break;
		case 2:
			return((outputs_5a_a & 0x02) >0);	
			break;
		case 3:
			return((outputs_5a_a & 0x04) >0);
			break;
		case 4:
			return((outputs_5a_a & 0x08) > 0);
			break;
		case 5:
			return((outputs_5a_a & 0x80) > 0);
			break;
		case 6:
			return((outputs_5a_a & 0x40) > 0);
			break;
		case 7:
			return((outputs_5a_a & 0x20) > 0);
			break;
		case 8:
			return ((outputs_5a_a & 0x10) > 0);
			break;
		case 9:
			return((outputs_5a_a & 0x08) > 0);
			break;
		case 10:
			return((outputs_5a_a & 0x04) > 0);
			break;
		case 11:
			return((outputs_5a_a & 0x02) > 0);
			break;
		case 12:
			return((outputs_5a_a & 0x01) > 0);
			break;
		case 13:
			return((outputs_5a_a & 0x10) > 0);
			break;
		case 14:
			return((outputs_5a_a & 0x20) > 0);
			break;
		case 15:
			return((outputs_5a_a & 0x40) > 0);
			break;
		case 16:
			return((outputs_5a_a & 0x80) > 0);
			break;
	}
}

ISR(TIMER1_COMPA_vect)
{
	pdu_clock++;
	DRS_counter++;
	/* Trigger CAN messages */
	if (pdu_clock % PDU_CAN_INTERVAL == 0) pdu_can_trigger = 1;

	/* Trigger input check */
	if (pdu_clock % PDU_INPUT_INTERVAL == 0) pdu_input_trigger = 1;

	if(DRS_counter >= 220){
		if(pdu_channel_enabled(16)){ //if channel 16 is high
			pdu_output_5a_disable(16); // disable ch 16
		}
		if(pdu_channel_enabled(15)){ //if channel 15 is high
			pdu_output_5a_disable(15); //disable ch 15
		}
	} 
		
	
	/* Trigger fuse check */
	pdu_fuse_trigger = 1;
}

//I'd probably just check in the ISR if an input is active, if so, increment a counter. 
//Once the counter reaches the max, it turns off the channel and resets the counter.