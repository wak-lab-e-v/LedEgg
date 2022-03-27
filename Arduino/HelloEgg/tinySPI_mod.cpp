// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Arduino hardware SPI master library for
// ATtiny24/44/84, ATtiny25/45/85, ATtiny261/461/861, ATtiny2313/4313.
//
// https://github.com/JChristensen/tinySPI
// Jack Christensen 24Oct2013

#include "tinySPI_mod.h"

void tinySPI::begin()
{
    USICR &= ~(_BV(USISIE) | _BV(USIOIE) | _BV(USIWM1));
    USICR |= _BV(USIWM0) | _BV(USICS1) | _BV(USICLK);
    SPI_DDR_PORT |= _BV(USCK_DD_PIN);   // set the USCK pin as output
    SPI_DDR_PORT |= _BV(DO_DD_PIN);     // set the DO pin as output
    SPI_DDR_PORT &= ~_BV(DI_DD_PIN);    // set the DI pin as input
}

void tinySPI::setDataMode(uint8_t spiDataMode)
{
    if (spiDataMode == SPI_MODE1)
        USICR |= _BV(USICS0);  // negative edge
    else
        USICR &= ~_BV(USICS0); // positive edge

   // if (spiDataMode == SPI_MODE2 || spiDataMode == SPI_MODE3) {
   //     digitalWrite(SCK, HIGH);
   // } else {
   //     digitalWrite(SCK, LOW);
   // }
}

uint8_t tinySPI::transfer(uint8_t spiData)
{
    USIDR = spiData;
    USISR = _BV(USIOIF);                // clear counter and counter overflow interrupt flag
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   // ensure a consistent clock period
    {
        while ( !(USISR & _BV(USIOIF)) ) USICR |= _BV(USITC);
    }
    return USIDR;
}

void tinySPI::end()
{
    USICR &= ~(_BV(USIWM1) | _BV(USIWM0));
}

uint8_t tinySPI::clockoutUSI2(uint8_t data)
{
    // Unlike other clockout methods, this one cannot rely on the
    // "external" clock source (USICS1) because it is too slow and
    // glitches. Instead, it uses software strobe explicitly.
    uint8_t strobe1 = _BV(USIWM0) | _BV(USITC);
    uint8_t strobe2 = _BV(USIWM0) | _BV(USITC) | _BV(USICLK);
    uint8_t usicr = USICR;
    bool mode1 = usicr & _BV(USICS0);
    USISR = _BV(USIOIF);  //clear counter and counter overflow interrupt flag
    USIDR = data;
    // Use asm to prevent instruction reordering.
    if (!mode1) {
        asm volatile("out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     :: [usicr] "I" (_SFR_IO_ADDR(USICR)),
                        [strobe1] "r" (strobe1),
                        [strobe2] "r" (strobe2));
    } else {
        asm volatile("out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     "out %[usicr], %[strobe1] \n\t"
                     "out %[usicr], %[strobe2] \n\t"
                     :: [usicr] "I" (_SFR_IO_ADDR(USICR)),
                        [strobe1] "r" (strobe1),
                        [strobe2] "r" (strobe2));
    }
    uint8_t retval = USIDR;
    USICR = usicr;
    return retval;
}

tinySPI SPI;
