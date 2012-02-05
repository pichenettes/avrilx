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
#include "avrlibx/system/init.h"
#include "avrlibx/system/timer.h"
#include "avrlibx/system/time.h"

using namespace avrlibx;

// 2-phase blink with interrupt on overflow and compare.
Timer<PortF, 0> timer;
Gpio<PortF, 0> led_a;
Gpio<PortF, 1> led_b;

ISR(TCF0_OVF_vect) {
  led_a.Toggle();
}

ISR(TCF0_CCA_vect) {
  led_b.Toggle();
}

int main(void) {
  SysInit();
  
  led_a.set_direction(OUTPUT);
  led_b.set_direction(OUTPUT);
  
  timer.set_prescaler(TIMER_PRESCALER_CLK_1024);
  timer.set_period(15625);
  timer.set_mode(TIMER_MODE_NORMAL);
  timer.set_channel<TIMER_CHANNEL_A>(15625 / 3);
  
  timer.EnableOverflowInterrupt(1);
  timer.EnableChannelInterrupt(TIMER_CHANNEL_A, 1);
  
  while (1) { }
}