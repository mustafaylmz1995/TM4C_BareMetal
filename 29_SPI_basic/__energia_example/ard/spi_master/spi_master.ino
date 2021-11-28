//ard mega COM7
#include <SPI.h>

#define SS 10
#define MOSI 11
#define MISO 12
#define SCK 13


void setup (void)
{
  pinMode(SS, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);

  digitalWrite(SCK, LOW);
  digitalWrite(MOSI, LOW);
  digitalWrite(SS, HIGH); 
   
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
} 
 
void loop (void)
{
  char c;
  digitalWrite(SS, LOW);   
 
  for (char p = 'A'; p<'z'; p++){
    SPI.transfer (p);
    delay(500);
  }
 
  digitalWrite(SS, HIGH);
 
  delay (1000); 
} 
