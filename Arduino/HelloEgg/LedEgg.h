#ifndef _LEDEGG_H_
#define _LEDEGG_H_

#include "tinySPI_mod.h"
#include <Arduino.h>

#define NUM_LED 24

uint8_t gamma8(uint8_t x);

class LedEgg
{
  public: 
    // Constructor to be used with SPI
    LedEgg();
    ~LedEgg();

    // SPI begin 
    //bool begin(uint8_t user_CSPin, uint32_t spiPortSpeed = 1000000, SPIClass &spiPort = SPI); 
    bool begin(uint8_t user_CSPin, uint32_t spiPortSpeed = 1000000, tinySPI &spiPort = SPI); 
    void WriteData(uint8_t _Byte1, uint8_t _Byte2, uint8_t _Byte3);
    uint8_t LEDarray[NUM_LED];
    void do_LESs();
		
  private:
    uint8_t _Output;
    uint32_t _spiPortSpeed; // Given sport speed. 
    uint8_t _cs;       // Chip select pin
    uint8_t _regValue; // Variable for returned register data. 
    uint8_t _spiWrite; // Variable used for SPI write commands. 

    //SPISettings mySpiSettings; 

    // SPI Classes
    //SPIClass *_spiPort;
    tinySPI  *_spiPort;

};
#endif

// SPI.beginTransaction(SPISettings(u8x8->bus_clock, MSBFIRST, internal_spi_mode));
