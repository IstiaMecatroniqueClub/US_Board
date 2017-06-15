//! \file main.cpp
//! \brief Main file
//! \author Baptiste Hamard,Franck Mercier, Remy Guyonneau
//! \date 2017 04 24
//!
//! Main file for the US card. To change the Ultra Sound identifier, just change the IDMSCAN constant before generating the .hex file.
#define FOSC           16000
#define F_CPU          16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "hcsr04.h"
#include "led.h"

// Configuration declaration for the LED wiring
#define LED_RED_PORT PORTB //!< The ATMega port where the red LED is wired
#define LED_RED_PIN  3     //!< The ATMega pin where the red LED is wired
#define LED_RED_POL  0     //!< The polarity of the red LED


// Configuration declaration for the LED wiring
#define US_TRIG_PORT    PORTC //!< The ATMega port where the trigger of the US is wired
#define US_TRIG_PIN     0     //!< The ATMega port where the trigger of the US is wired
#define US_ECHO_PORT    PORTD //!< The ATMega port where the echo of the US is wired
#define US_ECHO_PIN     6     //!< The ATMega port where the echo of the US is wired


#define IDMSGCAN 0x0C0 //!< the identifier of the US card

//! \fn void initCANBus()
//! \brief Intialize the CAN Bus.
//!
//! Function that initializes the CANBus according to the wiring and the needed speed (500kb/s).
//! It also enable the CAN module interruptions
void initCANBus(){
	// Enable of CAN transmit
	DDRC|=0x80; // Configuration of the standby pin as an output
	// (for a transceiver MCP2562 with the stby pin wired to the uC PC7)
	// Enable the MCP2562 (STANDBY to 0)
	PORTC &= 0x7F; // Activation of the MCP2562
	// (for a transceiver MCP2562 with the stby pin wired to the uC PC7)

	// Initialization of CAN module
	CANGCON  = (1 << SWRES); // Reset CAN module
	CANGCON  = 0x02; // Enable CAN module

	CANBT1 = 0x06; // Speed Config (500kb/s)
	CANBT2 = 0x04;
	CANBT3 = 0x13;

	CANHPMOB = 0x00; // No priority between the CAN MOB

	// Enable CAN interruptions and especially reception interruptions
	CANGIE |= 0xA0;
}

//! \fn void initCANMOB()
//! \brief Intialize the CAN MOB.
//!
//! Intialize the CAN MOB. A MOB can be seen as a "CAN socket".
//! In this case the MOB1 is used to receive remote request
//! and MOB0 is used to send the Ultra sound value
void initCANMOB(){
	CANPAGE = 0x10; // selection of MOB1 (reveice request)

	CANIDT4 = 0x04; // Config as reception remote (rtr = 1)
	CANIDT3 = 0x00;
	CANIDT2 = (uint8_t)( (IDMSGCAN & 0x00F)<< 5 ); // using the constant to configure the remote request identifier
	CANIDT1 = (uint8_t)( IDMSGCAN >> 3 );

	CANIDM4 = 0x04; // mask over the rtr value
	CANIDM3 = 0xFF; // mask over the identifier (we only want an interruption if the received CAN message is a remote request with the corresponding identifier)
	CANIDM2 = 0xFF;
	CANIDM1 = 0xFF;

	CANCDMOB = 0x80; // Config MOB as reception
	
	CANIE2 = 0x02; // enable the interruptions over the MOB 1

	sei(); // set enable interruption
}

Hcsr04 us_sensor(Output(&US_TRIG_PORT, US_TRIG_PIN), Input(&US_ECHO_PORT, US_ECHO_PIN)); //!< The Ultra Sound Sensor
Led led(&LED_RED_PORT, LED_RED_PIN, LED_RED_POL); //!< The RED Led

int main(void) {
	for(int i=0; i<4; i++){
		led.blink(100);
	}

	initCANBus(); // intialization of the CAN Bus
	initCANMOB(); // intialization of the CAN MOB

	while(1){
	}
}


//! \fn ISR(CAN_INT_vect)
//! \brief CAN interruption.
//! This function is called when an CAN interruption is raised.
//! When it appens, the US sensor value is readed and sent using the MOB 0
ISR(CAN_INT_vect){
	cli(); // disable the interruption (no to be disturbed when dealing with one)

	led.blink(50); // blink the LED

	CANPAGE = 0x10; // Selection of MOB 1
	CANSTMOB = 0x00; // Reset the status of the MOB
	CANCDMOB = 0x80; // Config as reception MOB
	CANIE2 = 0x02; // Enable the interuption over MOB 1 (for the next one)

	uint16_t dst = us_sensor.getDistance(); // get the distance of the Ultra Sound Sensor

	CANPAGE  = 0x00; //Selection of MOB 0

	CANIDT4 = 0x00; // Config as data (rtr = 0)
	CANIDT3 = 0x00;
	CANIDT2 = (uint8_t)( (IDMSGCAN & 0x00F)<< 5 ); // use the same identifier as the remote request
	CANIDT1 = (uint8_t)( IDMSGCAN >> 3 );

	CANMSG = (uint8_t)( (dst & 0xFF00) >> 8 ); // update the first byte of the CAN message data
	CANPAGE = 0x01; // move to next index for CANMSG (canpage : MOB|INDEX) (next byte of the data)
	CANMSG = (uint8_t)( dst & 0x00FF ); // update the second byte of the CAN message data

	CANCDMOB = 0x42; // send the message using the MOB 0

	sei(); // enable the interruptions
}