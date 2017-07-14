/*
 * pdu_can.h
 *
 * Created: 2017-04-22 10:09:14 AM
 *  Author: Darryl Ring
 */ 


#ifndef PDU_CAN_H_
#define PDU_CAN_H_


void pdu_can_init(void);
void pdu_can_send(uint8_t page, uint16_t id, volatile uint8_t msg[], uint8_t len);


#endif /* PDU_CAN_H_ */