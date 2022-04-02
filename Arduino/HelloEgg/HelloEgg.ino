#include "LedEgg.h"

LedEgg myLedEgg;
uint8_t brightness;

void setup() {
  CLKPR = 1<<CLKPCE;
  CLKPR = 0<<CLKPCE | 0<<CLKPS3 | 0<<CLKPS2 | 0<<CLKPS1 | 0<<CLKPS0;
    
  // put your setup code here, to run once:

  // pullups on for unused pins to minimize power consumption
  //pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  myLedEgg.begin(3);
  myLedEgg.WriteData( 255,  255,  255);
  for (uint8_t i = 0;i<NUM_LED; i++)
    myLedEgg.LEDarray[i] = 40;
}

void loop() {
  
   static uint16_t delays; 
   // put your main code here, to run repeatedly:
   if (!delays --)
   {
     brightness = brightness +1;
     delays = 15;
     
     //myLedEgg.LEDarray[0] = brightness;
     for (uint8_t i = 0;i<NUM_LED; i++)
     {
       myLedEgg.LEDarray[i] = (brightness = brightness + 5)/2;
       if (brightness > 128)
         brightness = brightness - 128;
     }
     //myLedEgg.LEDarray[7] = 255;
   }
   
   myLedEgg.do_LESs();
   
}
