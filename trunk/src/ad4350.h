#include <avr/io.h>
#include <stdlib.h>
void initAD4350();
void sendWord(uint32_t word);
void sendRegister0(uint32_t integer, uint32_t frac);
void sendRegister1(uint32_t prescaler, uint32_t phase, uint32_t mod);
void sendRegister2(uint32_t low_noise, uint32_t muxout,uint32_t reference_doubler,
			uint32_t rdiv2, uint32_t r_counter, uint32_t double_buff,
			uint32_t charge_pump_current_setting, uint32_t ldf, 
			uint32_t ldp, uint32_t pd_polarity, uint32_t power_down,
			uint32_t cp_three_state, uint32_t counter_reset);
void sendRegister3(uint32_t csr, uint32_t clk_div_mode, 
			uint32_t clock_divider_value);
void sendRegister4(uint32_t feedback_select, uint32_t divider_select,
			uint32_t band_select_clock_divider, 
			uint32_t vco_power_down, uint32_t mtld, 
			uint32_t aux_output_select, uint32_t aux_output_enable,
			uint32_t aux_output_power, uint32_t rf_output_enable, 
			uint32_t output_power);
void sendRegister5(uint32_t ld_pin_mode);
uint8_t readLockDetect();
void delay(uint32_t dly);
void setFrequency(uint64_t ref, uint64_t rf_out);
void test();
