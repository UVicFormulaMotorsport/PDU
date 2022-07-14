#include <avr/interrupt.h>
#include <avr/io.h>

#include "pdu.h"

#include "pdu_can.h"

uint16_t pdu_can_get_id(void);
void pdu_can_set_id(uint16_t id);
void pdu_can_set_mask(uint16_t mask);

void
pdu_can_init(void)
{
	/* Configure CAN_TX pin as output */
	DDRD |= (1 << DDD5);

	/* Reset the CAN controller */
	CANGCON = (1 << SWRES);

	/*
	  Configure CAN bit timing
	  Baud rate of 1 Mbps with a 16 MHz clock. See Table 19-2 on page 188 of the
	  data sheet.
	*/
	CANBT1 = 0x00;
	CANBT2 = 0x0C;
	CANBT3 = 0x36;

	/* Enable interrupts */
	CANGIE |= (1 << ENIT) | (1 << ENRX);
	CANIE2 |= (1 << IEMOB0);

	/* Configure message receive on page 0 */
	CANPAGE  = 0;
	CANCDMOB = 0x00;
	CANSTMOB = 0x00;

	pdu_can_set_id(0);
	pdu_can_set_mask(0);

	/* All input messages are 1 byte */
	CANCDMOB |= 8;

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

ISR(CANIT_vect)
{
	uint16_t id;
	uint8_t  len;
	uint8_t  idx;
	uint8_t  byte;

	CANPAGE = 0;

	id  = pdu_can_get_id();
	len = (CANCDMOB & 0x0F);

	/* F88RX->PDU: Enable or disable a channel */
	if (id == 0x710) {
		
		for (idx = 0; idx < len; idx++) {
			byte = CANMSG;
			
			// bits 0 - 3 specify the channel we are toggling
			// bit 4 signifies enable or disable, 1 - enable, 0 - disable
			// bit 5 signifies 20amp vs 5 amp, 1 - 20amp, 0 - 5amp
			
			// 20 amp output
			if(byte & 0x20)
			{
				// Enable
				if(byte & 0x10)
				{
					pdu_output_20a_enable(byte & 0x0F);
				}
				// Disable
				else
				{
					pdu_output_20a_disable(byte & 0x0F);
				}
			}
			// 5 amp output
			else
			{
				// Enable
				if(byte & 0x10)
				{
					pdu_output_5a_enable(byte & 0x0F);
				}
				// Disable
				else
				{
					pdu_output_5a_disable(byte & 0x0F);
				}
			}
		}
	}
	
	
	/* Re-enable transmission */
	CANSTMOB = 0;
	CANCDMOB |= (1 << CONMOB1);
}


//implement into ECU 

//two high current outputs that are unused, connect the two DRS pins to those
//swap the DRS open and DRS closed pins to another two PDU pins 

//branch and start committing 

//LCD backlight, storage, route JTAG to connector, anything else? CPU config? USB? 
//software challenge
//possibility to implement telemetry
//darryl has some software stuff done
//LINUX software person