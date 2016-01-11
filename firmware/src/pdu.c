/*
  This file is part of the UVic Formula Motorsports PDU project.

  Copyright (c) 2015 UVic Formula Motorsports

  This program is free software: you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include "pdu.h"


volatile uint32_t pdu_clock = 0;

volatile uint16_t pdu_can_clock = 0;
volatile uint8_t pdu_can_trigger = 0;

volatile uint8_t pdu_channel_status = 0;
volatile uint8_t pdu_channel_currents[6] = {0, 0, 0, 0, 0, 0};


void pdu_gpio_init(void);
void pdu_timer_init(void);


int
main(void)
{
	/* Enable global interrupts */
	sei();
	
	/* Initialize peripherals */
	pdu_adc_init();
	pdu_can_init();
	pdu_gpio_init();
	pdu_timer_init();

	for (;;) {
		pdu_adc_run();

		if (pdu_can_trigger) {
			pdu_can_send_system_status();
			pdu_can_send_channel_status();
			pdu_can_send_channel_currents();
			pdu_can_trigger = 0;
		}
	}

	return 0;
}


void
pdu_gpio_init(void)
{
	/* Configure driver switch pins as outputs */
	DDRB = (1 << DDB4);
	DDRC = (1 << DDC0) || (1 << DDC2);
	DDRD = (1 << DDD0) || (1 << DDD1) || (1 << DDD6) || (1 << DDD7);

	/* Configure discrete input pins as inputs with pull-ups */
	PORTB |= (1 << PORTB0) |  (1 << PORTB1);
	PORTD |= (1 << PORTD5);
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


void
pdu_channel_enable(channel_e channel)
{
	if      (channel == HC1) PORTD |= (1 << PORTD6);
	else if (channel == HC2) PORTD |= (1 << PORTD1);
	else if (channel == LC1) PORTB |= (1 << PORTB4);
	else if (channel == LC2) PORTD |= (1 << PORTD7);
	else if (channel == LC3) PORTC |= (1 << PORTC0);
	else if (channel == LC4) PORTD |= (1 << PORTD0);

	pdu_channel_status |= (1 << channel);
}


void
pdu_channel_disable(channel_e channel)
{
	if      (channel == HC1) PORTD &= ~(1 << PORTD6);
	else if (channel == HC2) PORTD &= ~(1 << PORTD1);
	else if (channel == LC1) PORTB &= ~(1 << PORTB4);
	else if (channel == LC2) PORTD &= ~(1 << PORTD7);
	else if (channel == LC3) PORTC &= ~(1 << PORTC0);
	else if (channel == LC4) PORTD &= ~(1 << PORTD0);

	pdu_channel_status &= ~(1 << channel);
}


void
pdu_channel_trip(channel_e channel)
{
	pdu_channel_disable(channel);
	pdu_can_send(0, PDU_CAN_ID_OUT_CHANNEL_TRIPPED, &channel, 1);
}


ISR(TIMER1_COMPA_vect)
{
	pdu_clock++;

	/* Trigger CAN messages */
	if (pdu_clock % PDU_CAN_INTERVAL == 0) pdu_can_trigger = 1;
}
