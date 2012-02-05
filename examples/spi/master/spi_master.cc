// Copyright 2011 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <avr/io.h>

#include "avrlibx/io/gpio.h"
#include "avrlibx/io/spi.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/timer.h"

using namespace avrlibx;

Timer<PortD, 0> timer;
SPIMaster<PortD, Gpio<PortD, 4>, MSB_FIRST, SPI_PRESCALER_CLK_2> dac_interface;

volatile uint16_t ramp;

ISR(TCD0_OVF_vect) {
  dac_interface.OptimisticWait();
  dac_interface.Strobe();
  ramp += 1024;
  uint16_t value = ramp >> 4;
  dac_interface.Overwrite(0x30 | (value >> 8));
  dac_interface.OptimisticWait();
  dac_interface.Overwrite(value & 0xff);
}

int main(void) {
  SysInit();
  
  dac_interface.Init();
  dac_interface.WriteWord(0, 0);
  
  timer.set_prescaler(TIMER_PRESCALER_CLK);
  timer.set_period(800);
  timer.set_mode(TIMER_MODE_NORMAL);
  
  timer.EnableOverflowInterrupt(1);

  while (1) { }
}
