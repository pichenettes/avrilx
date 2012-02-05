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

#include "avrlibx/devices/hd44780_lcd.h"
#include "avrlibx/io/gpio.h"
#include "avrlibx/io/parallel_io.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/timer.h"

using namespace avrlibx;

PWM<PortD, 0> contrast;
Gpio<PortD, 1> rw;
Hd44780Lcd<
    Gpio<PortD, 2>,
    Gpio<PortD, 3>,
    ParallelPort<PortD, 4, 7>,
    8, 1> lcd;

int main(void) {
  SysInit();
  
  rw.set_direction(OUTPUT);
  rw.Low();

  contrast.Init(8);
  contrast.set_value(50);

  lcd.Init();
  lcd.Write("hello!");
  lcd.Flush();
  while (1) {
  }
}
