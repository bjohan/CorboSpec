#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "corbomite.h"
#include "uart_command_lib.h"
#include "ad4350.h"

void testCommand()
{
	transmitStringP(PSTR("test command executed\r\n"));
}

void powerLed(uint8_t on)
{
	if(on)
		PORTD |= _BV(PD3);
	else
		PORTD &= ~_BV(PD3);
}

void centerFrequency(int32_t f)
{
	setFrequency(5e7, f<<4);
}

DIG_OUT("pwr", powerLed, powerLedCmd);

EVENT_OUT("cmd", testCommand, testCmd);

INFORMATION_HINT("test hest", testHint);
 
ANA_OUT("fc", "Hz", "Hz", "GHz", 137500000>>4, 4000000000>>4, centerFrequency, fre);

const EventData initEvent PROGMEM = {registeredEntries};

const CorbomiteEntry init PROGMEM = 
	{EVENT_OUT, internalId, (CorbomiteData*)&initEvent};

const CorbomiteEntry last PROGMEM = {LASTTYPE, "", 0};



const CorbomiteEntry * const entries[] PROGMEM = {
	&testHint,
	&fre,
	&testCmd,
	&powerLedCmd,
	&init, &last
};


int main(void) 
{
	initUsart();
	transmitStringP(PSTR("CorboSpec initialized\r\n"));
	DDRD |= _BV(PD3);
	PORTD |= _BV(PD3);
	delay(10000);
	initAD4350();
	test();
	setFrequency(5e7, 4200000000);
	while(1){
	//initAD4350();
		//sendWord(0x7FAA55FE);
		//if(readLockDetect())
		//	PORTD |= _BV(PD3);
		//else
		//	PORTD &= ~_BV(PD3);
		/*PORTD =0; 
		PORTD |= _BV(PD3);
		PORTD =0;*/
		//delay(10000);
		commandLine();
	}
}
