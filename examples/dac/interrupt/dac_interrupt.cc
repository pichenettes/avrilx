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

#include "avrlibx/io/dac.h"
#include "avrlibx/io/gpio.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/timer.h"
#include "avrlibx/system/time.h"

using namespace avrlibx;

Timer<PortF, 0> timer;
Gpio<PortF, 0> led;
DAC<PortB, true, true, DAC_REF_INTERNAL_1V> dac;

static uint16_t ramp;

ISR(TCF0_OVF_vect) {
  dac.Wait();
  dac.channel_0.Write(ramp & 0x0fff);
  dac.channel_1.Write((ramp + 2048) & 0x0fff);
  
  ramp += 32;
  if (ramp == 0) {
    led.Toggle();
  }
}

int main(void) {
  SysInit();

  dac.Init();
  led.set_direction(OUTPUT);
  timer.set_prescaler(TIMER_PRESCALER_CLK);
  timer.set_period(800);
  timer.set_mode(TIMER_MODE_NORMAL);
  timer.EnableOverflowInterrupt(1);

  while (1) { }
}