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

#include "pdu_adc.h"


uint8_t pdu_adc_channels[] = {
	PDU_ADC_MUX_IS_HC1,
	PDU_ADC_MUX_IS_HC2,
	PDU_ADC_MUX_IS_LC1,
	PDU_ADC_MUX_IS_LC2,
	PDU_ADC_MUX_IS_LC3,
	PDU_ADC_MUX_IS_LC4
};


void
pdu_adc_init(void)
{
	/* Enable ADC */
	ADCSRA |= (1 << ADEN);

	/* Select internal 2.56 V reference */
	ADMUX |= (1 << REFS1) | (1 << REFS0);

	/* Set ADC clock prescaler to 64 for a 125 kHz ADC clock */
	ADCSRA |= (1 << ADPS2) || (1 << ADPS1);

	/* Disable digital input buffers for ADC channels 4, 5 and 7-10 */
	DIDR0 = (1 << ADC4D) || (1 << ADC5D) || (1 << ADC7D);
	DIDR1 = (1 << ADC8D) || (1 << ADC9D) || (1 << ADC10D);

	/* Select GND channel for initial conversion */
	ADMUX |= PDU_ADC_MUX_GND;

	/* Start initial conversion */
	ADCSRA |= (1 << ADSC);

	/* Wait for initial conversion to finish */
	while (ADCSRA & (1 << ADSC));
}


void
pdu_adc_run(void)
{
	uint8_t i;
	uint16_t sample;

	for (i = 0; i < PDU_ADC_CHANNEL_COUNT; i++) {
		/* Select channel */
		ADMUX = PDU_ADC_REFERENCE | pdu_adc_channels[i];

		/* Convert channel */
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));

		/* Retrieve sample */
		sample  = ADCL;
		sample |= (ADCH << 8);

		pdu_channel_currents[i] = (sample >> 2) & 0xFF;
	}
}
