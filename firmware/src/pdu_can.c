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

#include "pdu_can.h"


void pdu_can_set_id(uint16_t id);
void pdu_can_set_mask(uint16_t mask);


void
pdu_can_init(void)
{
	/* Reset the CAN controller */
	CANGCON = (1 << SWRES);

	/*
	  Configure CAN bit timing

	  Baud rate of 1000 Kbps with an 8 MHz clock. See Table 19-2 on page 188 of
	  the data sheet.
	*/
	CANBT1 = 0x12; //0x00;
	CANBT2 = 0x04;
	CANBT3 = 0x13;

	/* Enable interrupts */
	CANGIE |= (1 << ENIT) | (1 << ENRX);
	CANIE2 |= (1 << IEMOB0);

	/* Configure message receive on page 0 */
	CANPAGE = 0;
	CANCDMOB = 0x00;
	CANSTMOB = 0x00;

	pdu_can_set_id(PDU_CAN_ID_BASE);
	pdu_can_set_mask(PDU_CAN_INPUT_ID_MASK);

	/* All input messages are 1 byte */
	CANCDMOB |= 1;

	/* Enable receive */
	CANCDMOB |= (1 << CONMOB1);

	/* Enable CAN controller */
	CANGCON |= (1 << ENASTB);
}


void pdu_can_send(uint8_t page, uint16_t id, volatile uint8_t msg[], uint8_t len)
{
	uint8_t i;

	CANPAGE = page << MOBNB0;

	CANCDMOB = 0x00;
	CANSTMOB = 0x00;

	pdu_can_set_id(id);

	for (i = 0; i < len; i++) {
		CANMSG = msg[i];
	}
	
	CANCDMOB |= len;
	CANCDMOB |= (1 << CONMOB0);

	while((CANSTMOB & (1 << TXOK)) != (1 << TXOK));
	CANCDMOB = 0x00;
	CANSTMOB = 0x00;
}


void
pdu_can_send_system_status(void)
{
	pdu_can_send(1, PDU_CAN_ID_OUT_SYSTEM_STATUS, NULL, 0);
}


void
pdu_can_send_channel_status(void)
{
	pdu_can_send(2, PDU_CAN_ID_OUT_CHANNEL_STATUS, &pdu_channel_status, 1);
}


void
pdu_can_send_channel_currents(void)
{
	pdu_can_send(3, PDU_CAN_ID_OUT_CHANNEL_CURRENTS, pdu_channel_currents, 6);
}


void
pdu_can_set_id(uint16_t id)
{
	CANIDT4 = 0;
	CANIDT3 = 0;
	CANIDT2 = (uint8_t)  id << 5;
	CANIDT1 = (uint16_t) id >> 3;
}

uint16_t
pdu_can_get_id(void)
{
	uint16_t id = 0;
	
	id |= (CANIDT2 >> 5);
	id |= (CANIDT1 << 3);

	return id;
}


void
pdu_can_set_mask(uint16_t mask)
{
	CANIDM4 = 0;
	CANIDM3 = 0;
	CANIDM2 = (uint8_t)  mask << 5;
	CANIDM1 = (uint16_t) mask >> 3;
}


ISR(CAN_INT_vect)
{
	uint16_t id;
	channel_e channel;

	CANPAGE = 0;

	id = pdu_can_get_id();
	channel = CANMSG;

	if (id == PDU_CAN_ID_IN_CHAN_ENABLE) {
		pdu_channel_enable(channel);
	} else if (id == PDU_CAN_ID_IN_CHAN_DISABLE) {
		pdu_channel_disable(channel);
	} else if (id == PDU_CAN_ID_IN_CHAN_DISABLE_ALL) {
	}
	
	/* Re-enable transmission */
	CANSTMOB = 0;
	CANCDMOB |= (1 << CONMOB1);
}