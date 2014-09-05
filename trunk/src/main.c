#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>



int main(void) 
{
	DDRD |= _BV(PD3);
	PORTD |= _BV(PD3);
	while(1);
}
