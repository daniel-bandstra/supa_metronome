/*
  TRNG control functions taken from:
  https://github.com/SapientHetero/TRNG-for-ATSAMD51J19A-Adafruit-Metro-M4-.git

  Which has the following copyright announcement:

  Copyright 2019 Ron Sutton
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef RANDOM_H
#define RANDOM_H

volatile uint32_t random_number;
volatile byte available_bytes = 0;

// This function sets up and starts the TRNG
void trngInit(void) {
  MCLK->APBCMASK.reg |= MCLK_APBCMASK_TRNG;    // Activate the CLK_TRNG_APB clock
  NVIC_SetPriority(TRNG_IRQn, 0);              // Set NVIC priority for TRNG to 0 
  NVIC_EnableIRQ(TRNG_IRQn);                   // Connect TRNG to NVIC 
  TRNG->INTENCLR.reg = TRNG_INTENCLR_DATARDY;  // Disable the TRNG interrupt
  TRNG->CTRLA.reg = TRNG_CTRLA_ENABLE;         // Enable the TRNG peripheral
  TRNG->INTENSET.reg = TRNG_INTENSET_DATARDY;  // Enable the TRNG interrupt
}

void TRNG_Handler()
{
  random_number = TRNG->DATA.reg;              // Read & store the TRNG data output
  available_bytes = 4;
  TRNG->INTFLAG.bit.DATARDY = 1;               // Clear the DATA READY interrupt flag
  TRNG->INTENCLR.reg = TRNG_INTENCLR_DATARDY;  // Disable the TRNG interrupt
}

byte random_byte() {
  byte output;
  if (available_bytes) {
    output = random_number & 255;
    random_number = random_number>>8;
    available_bytes--;
  } else {
    TRNG->INTENSET.reg = TRNG_INTENSET_DATARDY;  // Enable the TRNG interrupt
    while (!available_bytes) {}
    output = random_number & 255;
    random_number = random_number>>8;
    available_bytes--;
  }
  if (!available_bytes)
    TRNG->INTENSET.reg = TRNG_INTENSET_DATARDY;  // Enable the TRNG interrupt
  return output;
}

byte chance; // from 0 (not random) to 128 (about a 50:50 shot)

bool randomly_correct() {
  if (chance) {
    return random_byte() <= (255 - chance);
  } else {
    return true;
  }
}

#endif
