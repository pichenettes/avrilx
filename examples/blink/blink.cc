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
#include "avrlibx/system/time.h"

using namespace avrlibx;

Gpio<PortF, 0> led_a;
Gpio<PortF, 1> led_b;
Gpio<PortD, 5> control;

int main(void) {
  SysInit();
  led_a.set_direction(OUTPUT);
  led_b.set_direction(OUTPUT);
  control.set_direction(INPUT);
  control.set_mode(PORT_MODE_PULL_UP);
  
  led_a.High();
  led_b.Low();
  while (1) {
    if (control.value()) {
      led_a.Toggle();
      led_b.Toggle();
      ConstantDelay(100);
    }
  }
}
