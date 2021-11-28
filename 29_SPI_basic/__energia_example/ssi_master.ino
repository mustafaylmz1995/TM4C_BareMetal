#include <SPI.h>

#define chipSelectPin PD_2

SPIClass mySPI(2); //3 is SSI2 on port D3 2 1 0
//GPIO_PD3_SSI2CLK, GPIO_PD2_SSI2FSS, GPIO_PD1_SSI2XDAT0, GPIO_PD0_SSI2XDAT1


void setup (void)
{
  Serial.begin(9600);
  delay(1000); // wait a bit serial monitor to open

  Serial.println("DKTM4C129SerialFlashRDID setup");

  mySPI.begin();

  mySPI.setClockDivider(8);
  pinMode(chipSelectPin, OUTPUT); // have to set CS pin as output after spi.begin

}

void loop (void)
{

  uint8_t c;



  for (uint8_t p = 'A'; p <= 'z'; p++) {
    digitalWrite(chipSelectPin, LOW);
    mySPI.transfer (p);
    digitalWrite(chipSelectPin, HIGH);

    Serial.println(char(p));

    delay(500);
  }



  delay (1000);
}
