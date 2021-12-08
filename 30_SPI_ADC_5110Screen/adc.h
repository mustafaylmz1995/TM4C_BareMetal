#ifndef __ADC_H__
#define __ADC_H__




void enable_clk(void);
void sample_seq_conf(void);
void Timer0_init(void);

void start_ADC_temp(void);
unsigned int temp_result(void);




#endif //__ADC_H__

