#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "corbomite.h"
#include "uart_command_lib.h"
#include "ad4350.h"
uint8_t cwEnable = 0;
uint8_t sweepEnable =0;
uint64_t startFreq = 135e6;
uint64_t stopFreq = 135e6;
uint64_t centerFreq = 135e6;
uint64_t steps = 1;
uint64_t stepSize = 0;
uint32_t currentStep = 0;
uint32_t currentFreq = 0;


void setSteps(int32_t s)
{
	steps = s;
}

void testCommand()
{
	if(cwEnable)
		return;
	//stopFreq = 252e6;//((uint64_t)f)<<4;
	//startFreq = 248e6;// ((uint64_t)f)<<4;
	//setFrequency(5e7, 666666666);
	//transmitStringP(PSTR("test command executed\r\n"));
	if(startFreq > stopFreq){
		stepSize = startFreq;
		startFreq = stopFreq;
		stopFreq = stepSize;
	}
	stepSize = stopFreq - startFreq;
	stepSize /= steps;
	if(stepSize == 0)
		stepSize = 1;
	sweepEnable = 1;
	currentStep = 0;
	currentFreq = startFreq;
}

void powerLed(uint8_t on)
{
	if(on)
		PORTD |= _BV(PD3);
	else
		PORTD &= ~_BV(PD3);
}

void toggleCw(uint8_t cw)
{
	cwEnable = cw;
	//if(cwEnable)
	//	setFrequency(5e7, currentFreq);
}

void setCenterFrequency(int32_t f)
{
	centerFreq = ((uint64_t)f);
	//if(cwEnable)
	//	setFrequency(5e7, currentFreq);
}

void setStopFrequency(int32_t f)
{
	stopFreq = ((uint64_t)f);
}

void setStartFrequency(int32_t f)
{
	startFreq = ((uint64_t)f);
}

void initAdc()
{
	ADMUX = _BV(REFS0) | _BV(MUX0); //Setup adc using external reference and ch1
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	DIDR0 |= _BV(ADC1D);
	DIDR0 |= _BV(ADC0D);
}

uint16_t readAdc(uint8_t ch)
{
	ADMUX = (ADMUX&0xF8)| (ch&7);
	ADCSRA |= _BV(ADSC);
	while(ADCSRA & _BV(ADSC));
	return ADC;
}
DIG_OUT("pwr", powerLed, powerLedCmd);
DIG_OUT("cw", toggleCw, cwCmd);
DIG_IN("lck", lockDetect);
EVENT_OUT("sweep", testCommand, testCmd);

INFORMATION_HINT("test hest", testHint);
 
ANA_OUT("fstart", "Hz", "Hz", "GHz", 137500000, 2000000000, setStartFrequency, fre);
ANA_OUT("fstop", "Hz", "Hz", "GHz", 137500000, 2000000000, setStopFrequency, stop);
ANA_OUT("steps", "stp", "stp", "stp", 1, 4000, setSteps, stepsWidget);
ANA_OUT("fcenter", "Hz", "Hz", "GHz", 137500000, 2000000000, setCenterFrequency, center);
TRACE_IN("plot", "Hz", "1", "dB", "1", plot);

ANA_IN("pin", "dBm", "-90", "20", 0, 1023, inputPower);
const EventData initEvent PROGMEM = {registeredEntries};

const CorbomiteEntry init PROGMEM = 
	{EVENT_OUT, internalId, (CorbomiteData*)&initEvent};

const CorbomiteEntry last PROGMEM = {LASTTYPE, "", 0};



const CorbomiteEntry * const entries[] PROGMEM = {
	&testHint,
	&fre,
	&stop,
	&stepsWidget,
	&center,
	&testCmd,
	&cwCmd,
	&powerLedCmd,
	&inputPower,
	&lockDetect,
	&plot,
	&testHint,
	&init, &last
};


int main(void) 
{
	uint16_t v=0;
	initAdc();
	initUsart();
	transmitStringP(PSTR("start\r\n"));
	DDRD |= _BV(PD3);
	PORTD |= _BV(PD3);
	initAD4350();
	setFrequency(5e7, 135e6);
	uint64_t lastFreq = centerFreq;
	while(1){
		commandLine();
		if(cwEnable){
			waitTransmissionIdle();
			transmitAnalogIn(&inputPower, readAdc(1));
			if(lastFreq != centerFreq){
				lastFreq = centerFreq;
				setFrequency(5e7, centerFreq);
			}
		} else {
			if(sweepEnable){
				setFrequency(5e7, currentFreq);
				waitTransmissionIdle();
				v = readAdc(1);
				transmitTraceIn(&plot, currentFreq, v);
				if(currentStep > steps)
					sweepEnable = 0;
				currentFreq += stepSize;
				currentStep++;
			}		
		}
	}

}
