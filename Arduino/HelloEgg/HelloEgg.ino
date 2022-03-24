#include "LedEgg.h"

LedEgg myLedEgg;

void setup() {
  // put your setup code here, to run once:
  myLedEgg.beginSPI(3);
  myLedEgg.WriteData(0xAD,0xAD,0xAD);

}

void loop() {
  // put your main code here, to run repeatedly:

}
