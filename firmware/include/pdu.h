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

#ifndef __PDU_H__
#define __PDU_H__


#include <stdint.h>
#include <string.h>

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#include "pdu_adc.h"
#include "pdu_can.h"


typedef enum {
	HC1 = 0x01,
	HC2 = 0x02,
	LC1 = 0x04,
	LC2 = 0x08,
	LC3 = 0x10,
	LC4 = 0x20
} channel_e;


void pdu_channel_enable(channel_e channel);
void pdu_channel_disable(channel_e channel);


volatile uint32_t pdu_clock;

volatile uint8_t pdu_can_trigger;

volatile uint8_t pdu_channel_status;
volatile uint8_t pdu_channel_currents[6];


#endif
