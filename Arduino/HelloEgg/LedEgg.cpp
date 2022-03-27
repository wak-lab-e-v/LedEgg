/*
  This is a library for the ASM AS3935 Franklin Lightning Detector
  By: Elias Santistevan
  SparkFun Electronics
  Date: January, 2019
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
*/


#include "LedEgg.h"

// Default constructor, to be used with SPI
LedEgg::LedEgg() { }

//LedEgg::~LedEgg() { _spiPort->endTransaction();}
LedEgg::~LedEgg() { }

static const uint8_t PROGMEM _GammaTable[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
   13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
   20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
   30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
   42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
   58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
   76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
   97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
  122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
  150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
  182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
  218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255};

static uint8_t gamma8(uint8_t x) {
    return pgm_read_byte(&_GammaTable[x]); // 0-255 in, 0-255 out
  }

//bool LedEgg::beginSPI(uint8_t user_CSPin, uint32_t spiPortSpeed, SPIClass &spiPort) 
bool LedEgg::begin(uint8_t user_CSPin, uint32_t spiPortSpeed, tinySPI &spiPort) 
{
  // Startup time requires 2ms for the LCO and 2ms more for the RC oscillators
  // which occurs only after the LCO settles. See "Timing" under "Electrical
  // Characteristics" in the datasheet.  
  delay(4);
  // I'll be using this as my indicator that SPI is to be used and not I2C.   
  _spiPort = &spiPort; 
  _cs = user_CSPin;
  pinMode(_cs, OUTPUT); 
  digitalWrite(_cs, HIGH);// Deselect the Lightning Detector. 

  
  // Bit order is different for ESP32
  // mySpiSettings = SPISettings(spiPortSpeed, MSBFIRST, SPI_MODE1);  
  

  _spiPort->begin();


  //LEDarray = new uint8_t[NUM_LED];

  return true; 
}

void LedEgg::do_LESs()
{
  uint8_t j;
  for (uint8_t i=0; i<128;i++)
  {
    
    digitalWrite(_cs, LOW); // Start communication
    for (j=16; j<24;j++)
    {
      if (i<gamma8(LEDarray[j]))
        _Output = (_Output >> 1) | 0x80;
      else
        _Output = (_Output >> 1);
    }  
    _spiPort->transfer(_Output); // Byte 1 
    for (j=8; j<16;j++)
    {
      if (i<gamma8(LEDarray[j]))
        _Output = (_Output >> 1) | 0x80;
      else
        _Output = (_Output >> 1);
    }  
    _spiPort->transfer(_Output); // Byte 2 
    for (j=0; j<8;j++)
    {
      if (i<gamma8(LEDarray[j]))
        _Output = (_Output >> 1) | 0x80;
      else
        _Output = (_Output >> 1);
    }  
    _spiPort->transfer(_Output); // Byte 3 
    digitalWrite(_cs, HIGH); // End communcation
  }   
  /* for (uint8_t i=0; i<255;i++)
   {
     _Output[0] = _Output[1] = _Output[2] = 0;
     for (uint8_t j=0; j<NUM_LED;j++)
     {
       uint8_t idx = j >> 3;
       if (i<gamma8(LEDarray[j]))
         _Output[idx] = (_Output[idx] >> 1) | 0x80;
       else
         _Output[idx] = (_Output[idx] >> 1);
     }  
     WriteData( _Output[0],  _Output[1],  _Output[2]);
   }  
  */
}
 
void LedEgg::WriteData(uint8_t _Byte1, uint8_t _Byte2, uint8_t _Byte3)
{
	digitalWrite(_cs, LOW); // Start communication
	_spiPort->transfer(_Byte1); // Byte 1
	_spiPort->transfer(_Byte2); // Byte 2
	_spiPort->transfer(_Byte3); // Byte 3
	digitalWrite(_cs, HIGH); // End communcation

}
