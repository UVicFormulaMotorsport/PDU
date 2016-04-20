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


#define PDU_INPUT_INTERVAL 10


typedef enum {
	HC1 = 0,
	HC2 = 1,
	LC1 = 2,
	LC2 = 3,
	LC3 = 4,
	LC4 = 5
} channel_e;

// Initiate a struct for current and delay (timer)
typedef struct {
    uint16_t current_fuse;
    uint16_t current_inrush;
    uint16_t delay_fuse;
    uint16_t delay_inrush;
} pdu_channel_t;

// 6 channels
pdu_channel_t pdu_channels[] = {
	{
		30,
		50,
		1000,
		100
	},
	{
		30,
		50,
		1000,
		100
	},
	{
		30,
		50,
		1000,
		100
	},
	{
		30,
		50,
		1000,
		100
	},
	{
		30,
		50,
		1000,
		100
	},
	{
		30,
		50,
		1000,
		100
	},

};

void pdu_channel_enable(channel_e channel);
void pdu_channel_disable(channel_e channel);


volatile uint32_t pdu_clock;

volatile uint8_t pdu_can_trigger;

volatile uint8_t pdu_channel_status[8];
volatile uint8_t pdu_channel_currents[8];

volatile uint8_t pdu_soft_input_status[8];
volatile uint8_t pdu_soft_output_status[8];

uint8_t pdu_input_status[3];


#endif
