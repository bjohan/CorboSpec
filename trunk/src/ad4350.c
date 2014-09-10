#include "ad4350.h"
#define SAT1(x) if((x) > 1) (x) = 1

void initAD4350()
{
	DDRB |= _BV(PB5) | _BV(PB4) | _BV(PB3); //Outputs for data interface
	DDRC &= ~_BV(PC0); //Input for lock detect
	
	/*sendRegister5(3);
	sendRegister4(1, 3, 200, 0, 0, 0, 0, 0, 1, 2);
	sendRegister3(0,0,150);
	sendRegister2(0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0);
	sendRegister1(1,2,2);
	sendRegister0(160,0);*/
}

void test()
{

//Register 0
uint32_t integer = 160;
uint32_t frac = 0;

//Register 1
uint32_t prescaler = 1;
uint32_t phase = 2;
uint32_t mod = 2;

//Register 2
uint32_t low_noise = 0;
uint32_t muxout = 0;
uint32_t reference_doubler = 0;
uint32_t rdiv2 = 1;
uint32_t r_counter = 1;
uint32_t double_buff = 0;
uint32_t charge_pump_current_setting = 0;
uint32_t ldf = 1;
uint32_t ldp = 0;
uint32_t pd_polarity = 1;
uint32_t power_down = 0;
uint32_t cp_three_state = 0;
uint32_t counter_reset = 0;

//Register 3
uint32_t csr = 0;
uint32_t clk_div_mode = 0;
uint32_t clock_divider_value = 150;

//Register 4
uint32_t feedback_select = 1;
uint32_t divider_select = 3;
uint32_t band_select_clock_divider = 200;
uint32_t vco_power_down = 0;
uint32_t mtld = 0;
uint32_t aux_output_select = 0;
uint32_t aux_output_enable = 0;
uint32_t aux_output_power = 0;
uint32_t rf_output_enable = 1;
uint32_t output_power = 2;

//Register 5
uint32_t ld_pin_mode = 3;


sendRegister5(ld_pin_mode);
sendRegister4(feedback_select, divider_select,band_select_clock_divider, 
		vco_power_down, mtld, aux_output_select, aux_output_enable,
		aux_output_power, rf_output_enable, output_power);
sendRegister3(csr, clk_div_mode, clock_divider_value);
sendRegister2(low_noise, muxout,reference_doubler, rdiv2, r_counter, 
		double_buff, charge_pump_current_setting, ldf, ldp, pd_polarity,
		power_down, cp_three_state, counter_reset);
sendRegister1(prescaler, phase, mod);
sendRegister0(integer, frac);
/*	sendRegister5(3);
	sendRegister4(1, 3, 200, 0, 0, 0, 0, 0, 1, 2);
	sendRegister3(0,0,150);
	sendRegister2(0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0);
	sendRegister1(1,2,2);
	sendRegister0(160,0);*/
}


void setFrequency(uint32_t ref, uint32_t rf_out)
{
/*	uint32_t F_pfd = 100e3;
	uint32_t D=0;
	uint32_t T=0; //Prescaler???
	//First select F_pfd
	if(ref < 30e6)
		D = 1;
	R = ref*(1+d)/(F_pfd*(1+T));
	if(R > 1023){ //R is too high, then set T to divide by two
		T = 1;
		R = ref*(1+d)/(F_pfd*(1+T));
		if(R > 1023){ //If still to high, disable the doubler
			D = 0;
			R = ref*(1+d)/(F_pfd*(1+T));
		}
	}

	if(R > 1023){
		transmitStringP(PSTR("TODO, illegal Rselect another F_pfd\r\n"));
		return;
	}
	
	

	//RF_out = (INT + FRAC/MOD)*F_pfd
	uint32_t INT = rf_out/f_pfd;

	if(INT < 75){
		transmitStringP(PSTR("TODO, illegal INT select anoter F_pfd\r\n"));
		return;
	}
	
	uint32_t FRAC = 0;
	uint32_t MOD = 0;	

	//F_pfd = ref*(1+D)/(R*(1+T))
*/
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
	
		//delay(10);
	for(uint32_t b = 32 ; b > 0 ; b--){
		PORTB &= ~_BV(PB5); //Clear CLK
		//delay(10);
		if(word & 0x80000000)
			PORTB |= _BV(PB4); //Set data if a one should be sent
		else
			PORTB &= ~_BV(PB4); //Clear data if a zero should be sent
		//delay(10);
		PORTB |= _BV(PB5); //Set CLK
		//delay(10);
		word <<= 1;
	}

		//delay(10);
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
		| (band_select_clock_divider << 12) | (vco_power_down << 11)
		| (mtld << 10) | (aux_output_select << 9) 
		| (aux_output_enable << 8) | (aux_output_power << 6)
		| (rf_output_enable << 5) | (output_power << 3) | 0x04);
}

void sendRegister5(uint32_t ld_pin_mode)
{
	ld_pin_mode &= 0x3;
	sendWord( (ld_pin_mode << 22) | 0x180005);
}
