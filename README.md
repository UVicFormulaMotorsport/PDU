# PDU

The UVic Formula Motorsports Power Distribution Unit (PDU) replaces traditional
fuses and relays with solid state drivers switched and monitored by a
microcontroller. The PDU is intended to distribute power to all of the car's
electrical systems. Two high current (15+ A) outputs will run the radiator fan
and fuel pump, while four low current (6 A) outputs power the engine control
unit (ECU) and data acqusition systems (DAQs).

The PDU makes use of Infineon's PROFET line of "Smart" automotive high side
switches to drive the various loads. These switches consist of a DMOS power
transistor and logic circuitry to provide built-in protection against overload,
overtemperature, and short circuit. In addition, they provide current sense
capabilities allowing a microcontroller to monitor the load current over time
and open the switch if current exceeds a specified value.

Three digital inputs combined with a CAN transceiver provide multiple methods to
communicate with the PDU. The digital inputs can be used to trigger various
events and ultimately open or close switches. CAN can be used for the same
purposes as well as logging load current data.

## Specifications

### Electrical

### Mechanical