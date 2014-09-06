#include "ad4350.h"
#define SAT1(x) if((x) > 1) (x) = 1

void initAD4350()
{
	DDRB |= _BV(PB5) | _BV(PB4) | _BV(PB3); //Outputs for data interface
	DDRC &= ~_BV(PC0); //Input for lock detect
	sendRegister5(3);
	sendRegister4(0, 0, 1, 0, 0, 0, 0, 0, 1, 0);
	sendRegister3(0,0,0);
	sendRegister2(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);
	sendRegister1(0, 1, 2);
	sendRegister0(46, 0);
}

uint8_t readLockDetect()
{
	return PORTC & 0x01;
}

void delay(uint32_t dly)
{
	while(dly--);
}

void sendWord(uint32_t word)
{
	PORTB &= ~_BV(PB3); //Clear LE
	
	for(uint32_t b = 32 ; b > 0 ; b--){
		PORTB &= ~_BV(PB5); //Clear CLK
		delay(10);
		if(word & 0x80000000)
			PORTB |= _BV(PB4); //Set data if a one should be sent
		else
			PORTB &= ~_BV(PB4); //Clear data if a zero should be sent
		delay(10);
		PORTB |= _BV(PB5); //Set CLK
		delay(10);
		word <<= 1;
	}

	PORTB |= _BV(PB3); //Set LE
}

void sendRegister0(uint32_t integer, uint32_t frac)
{
	integer &= 0xFFFF;
	frac &= 0xFFF;
	if(integer < 23) integer = 23;
	sendWord((integer << 15) | (frac << 3) | 0);
}

void sendRegister1(uint32_t prescaler, uint32_t phase, uint32_t mod)
{
	phase &= 0xFFF;
	mod &= 0xFFF;
	SAT1(prescaler);
	sendWord( (prescaler<<27) | (phase << 15) | (mod <<3) | 1);
}

void sendRegister2(uint32_t low_noise, uint32_t muxout,uint32_t reference_doubler,
			uint32_t rdiv2, uint32_t r_counter, uint32_t double_buff,
			uint32_t charge_pump_current_setting, uint32_t ldf, 
			uint32_t ldp, uint32_t pd_polarity, uint32_t power_down,
			uint32_t cp_three_state, uint32_t counter_reset)
{
	low_noise &= 0x3;
	muxout &= 0x7;
	SAT1(reference_doubler);
	SAT1(rdiv2);
	r_counter &= 0x3FF;
	SAT1(double_buff);
	charge_pump_current_setting &= 0xF;
	SAT1(ldf);
	SAT1(ldp);
	SAT1(pd_polarity);
	SAT1(power_down);
	SAT1(cp_three_state);
	SAT1(counter_reset);
	sendWord((low_noise << 29) | (muxout << 26) | (reference_doubler << 25)
		| (rdiv2 << 24) | (r_counter << 14) | (double_buff << 13) 
		| (charge_pump_current_setting << 9) | (ldf << 8) | (ldp << 7)
		| (pd_polarity << 6) | (power_down << 5) | (cp_three_state << 4)
		| (counter_reset << 3) | 0x02);
	
}

void sendRegister3(uint32_t csr, uint32_t clk_div_mode, 
			uint32_t clock_divider_value)
{
	SAT1(csr);
	clk_div_mode &= 0x3;
	clock_divider_value &= 0xFFF;
	sendWord( (csr << 18) | (clk_div_mode << 15) 
		| (clock_divider_value << 3) | 0x03);
}

void sendRegister4(uint32_t feedback_select, uint32_t divider_select,
			uint32_t band_select_clock_divider, 
			uint32_t vco_power_down, uint32_t mtld, 
			uint32_t aux_output_select, uint32_t aux_output_enable,
			uint32_t aux_output_power, uint32_t rf_output_enable, 
			uint32_t output_power)
{
	SAT1(feedback_select);
	if(divider_select > 5 ) divider_select = 5;
	band_select_clock_divider &= 0xFF;
	if(band_select_clock_divider == 0) band_select_clock_divider = 1;
	SAT1(vco_power_down);
	SAT1(mtld);
	SAT1(aux_output_select);
	SAT1(aux_output_enable);
	aux_output_power &= 0x3;
	SAT1(rf_output_enable);
	output_power &= 0x3;
	sendWord( (feedback_select << 23) | (divider_select << 20) 
		| (band_select_clock_divider << 23) | (vco_power_down << 11)
		| (mtld << 10) | (aux_output_select << 9) 
		| (aux_output_enable << 8) | (aux_output_power << 6)
		| (rf_output_enable << 5) | (output_power << 3) | 0x04);
}

void sendRegister5(uint32_t ld_pin_mode)
{
	ld_pin_mode &= 0x3;
	sendWord( (ld_pin_mode << 22) | 0x05);
}
