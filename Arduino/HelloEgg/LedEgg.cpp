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

LedEgg::~LedEgg() { _spiPort->endTransaction();}



bool LedEgg::beginSPI(uint8_t user_CSPin, uint32_t spiPortSpeed, SPIClass &spiPort) 
{
  // Startup time requires 2ms for the LCO and 2ms more for the RC oscillators
  // which occurs only after the LCO settles. See "Timing" under "Electrical
  // Characteristics" in the datasheet.  
  delay(4);
  // I'll be using this as my indicator that SPI is to be used and not I2C.   
  _spiPort = &spiPort; 
  _spiPortSpeed = spiPortSpeed; // Make sure it's not 500kHz or it will cause feedback with antekknna.
  _cs = user_CSPin;
  pinMode(_cs, OUTPUT); 
  digitalWrite(_cs, HIGH);// Deselect the Lightning Detector. 

  
  // Bit order is different for ESP32
#ifdef ESP32 
mySpiSettings = SPISettings(spiPortSpeed, SPI_MSBFIRST, SPI_MODE1);  
#else
mySpiSettings = SPISettings(spiPortSpeed, MSBFIRST, SPI_MODE1);  
#endif
  
#if ARDUINO >= 10600
    _spiPort->beginTransaction(mySpiSettings);
#else
    _spiPort->begin();
   /*   
    if ( u8x8->display_info->sck_pulse_width_ns < 70 )
	_spiPort->setClockDivider( SPI_CLOCK_DIV2 );
      else if ( u8x8->display_info->sck_pulse_width_ns < 140 )
	_spiPort->setClockDivider( SPI_CLOCK_DIV4 );
      else
   */		  
	_spiPort->setClockDivider( SPI_CLOCK_DIV8 );
    _spiPort->setDataMode(internal_spi_mode);
    _spiPort->setBitOrder(MSBFIRST);
#endif

  return true; 
}

 
void LedEgg::WriteData(uint8_t _Byte1, uint8_t _Byte2, uint8_t _Byte3)
{
	digitalWrite(_cs, LOW); // Start communication
	_spiPort->transfer(_Byte1); // Byte 1
	_spiPort->transfer(_Byte2); // Byte 2
	_spiPort->transfer(_Byte3); // Byte 3
	digitalWrite(_cs, HIGH); // End communcation

}
