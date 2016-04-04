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

#ifndef __PDU_CAN_H__
#define __PDU_CAN_H__


/*
  Base CAN message ID

  All CAN message IDs are specified relative to this base ID.
*/
#define PDU_CAN_ID_BASE 0x700

/*
  CAN message ID offsets

  The CAN message ID is calculated by adding the offset to the base ID.
*/
#define PDU_CAN_ID_STATUS   (0x01 + PDU_CAN_ID_BASE)
#define PDU_CAN_ID_CURRENTS (0x05 + PDU_CAN_ID_BASE)
#define PDU_CAN_ID_EXTRAS   (0x09 + PDU_CAN_ID_BASE)
#define PDU_CAN_ID_SOFT_IN  (0x10 + PDU_CAN_ID_BASE)
#define PDU_CAN_ID_SOFT_OUT (0x18 + PDU_CAN_ID_BASE)

/*
  CAN input message ID mask

  This mask is used to determine which messages are accepted. Incoming message
  IDs are compared to a given ID tag and if the difference ORred with the mask
  equals 0x7FF, the message is accepted.

  (id XNOR id_tag) OR (NOT id_mask) == 0x7FF
*/
#define PDU_CAN_INPUT_ID_MASK 0x780

/*
  CAN message interval (in milliseconds)

  The system and channel status messages will be sent after every interval using
  the counter from Timer/Counter0.

  Default is 10 ms for a 100 Hz update rate
*/
#define PDU_CAN_INTERVAL 10


void pdu_can_init(void);

void pdu_can_send(uint8_t page, uint16_t id, volatile uint8_t msg[], uint8_t len);

void pdu_can_send_status(void);


#endif
