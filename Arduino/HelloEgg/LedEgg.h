#ifndef _LEDEGG_H_
#define _LEDEGG_H_

#include <SPI.h>
#include <Arduino.h>

class LedEgg
{
  public: 
    // Constructor to be used with SPI
    LedEgg();
    ~LedEgg();

    // SPI begin 
    bool beginSPI(uint8_t user_CSPin, uint32_t spiPortSpeed = 1000000, SPIClass &spiPort = SPI); 
    void WriteData(uint8_t _Byte1, uint8_t _Byte2, uint8_t _Byte3);
		
  private:

    uint32_t _spiPortSpeed; // Given sport speed. 
    uint8_t _cs;       // Chip select pin
    uint8_t _regValue; // Variable for returned register data. 
    uint8_t _spiWrite; // Variable used for SPI write commands. 

    SPISettings mySpiSettings; 

    // SPI Classes
    SPIClass *_spiPort; 

};
#endif

// SPI.beginTransaction(SPISettings(u8x8->bus_clock, MSBFIRST, internal_spi_mode));
