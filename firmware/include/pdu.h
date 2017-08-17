/*
 * pdu.h
 *
 * Created: 2017-04-22 10:09:56 AM
 *  Author: Darryl Ring
 */ 


#ifndef PDU_H_
#define PDU_H_

#define F_CPU 16000000UL

#include <util/delay.h>


/****************************************

20 A channels
Port A (7: Ch6, 6: Ch5, 5: Ch8, 4: Ch7, 3: Ch2, 2: Ch1, 1: Ch4, 0: Ch3)

5 A channels
GPA (7: Ch5, 6: Ch6, 5: Ch7, 4: Ch8, 3: Ch4, 2: Ch3, 1: Ch2, 0: Ch1)
GPB (7: Ch16, 6: Ch15, 5: Ch14, 4: Ch13, 3: Ch9, 2: Ch10, 1: Ch11, 0: Ch12)

Inputs
GPA (7: In5, 6: In6, 5: In7, 4: In8, 3: In4, 2: In3, 1: In2, 0: In1)
GPB (7: Ch16, 6: Ch15, 5: Ch14, 4: Ch13, 3: Ch9, 2: Ch10, 1: Ch11, 0: Ch12)

*****************************************/

#define DEFAULT_20A_OUTPUTS 0x20; // ECU on by default
#define DEFAULT_5A_OUTPUTS_A 0xF7; // All non-switched channels on by default
#define DEFAULT_5A_OUTPUTS_B 0xFF; // ALl non-switched channels on by default

#define PDU_INPUT_INTERVAL 10


void pdu_output_20a_enable(uint8_t output);
void pdu_output_20a_disable(uint8_t output);

void pdu_output_5a_enable(uint8_t output);
void pdu_output_5a_disable(uint8_t output);

volatile uint32_t pdu_clock;
volatile uint32_t DRS_counter;

volatile uint8_t pdu_can_trigger;

#endif /* PDU_H_ */