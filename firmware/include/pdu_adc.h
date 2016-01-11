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

#ifndef __PDU_ADC_H__
#define __PDU_ADC_H__


/* How many channels to convert each ADC run */
#define PDU_ADC_CHANNEL_COUNT 6

/* ADC reference source */
#define PDU_ADC_REFERENCE (0x03 << 6)

/*
  ADC multiplexer channels

  Determines which ADC channels are connected to which current sense outputs.
*/
#define PDU_ADC_MUX_IS_HC1 0x09
#define PDU_ADC_MUX_IS_HC2 0x0A
#define PDU_ADC_MUX_IS_LC1 0x05
#define PDU_ADC_MUX_IS_LC2 0x08
#define PDU_ADC_MUX_IS_LC3 0x07
#define PDU_ADC_MUX_IS_LC4 0x04
#define PDU_ADC_MUX_GND    0x12



void pdu_adc_init(void);
void pdu_adc_run(void);


#endif
