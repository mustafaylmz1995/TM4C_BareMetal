#include "TM4C129.h"                    // Device header

#include "LCD5110_Basic.h"
#include "adc.h"
#include "delay.h"
#include <string.h>


extern unsigned char SmallFont[];

//Flash temizlendikten sonra tekrar yüklenir

int main(void){
	
	LCD5510_initLCD(65);
	delay(500*MS);
	
	LCD5110_disableSleep();
	delay(100*MS);
	
	LCD5110_invert(True);
	delay(500*MS);
	LCD5110_invert(False);
	delay(500*MS);
	
	LCD5110_clrScr();
	
	LCD5110_setFont(SmallFont);
	
	
	LCD5110_print("Mustafa Yilmaz", CENTER, 0);
	LCD5110_print("HAVELSAN A.S.", CENTER, 16);
	LCD5110_print("02.12.2021", CENTER, 32);
	delay(1000*MS);

	start_ADC_temp();
	
	while(1){
		unsigned temp = temp_result();
		char output[50];
		sprintf(output, "%d *C", temp);
		
		LCD5110_clrScr();
		LCD5110_print(output, CENTER, 16);
		
		delay(1000*MS);
	}

	
	
}






