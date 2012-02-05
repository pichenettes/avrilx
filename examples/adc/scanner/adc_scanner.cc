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

#include "avrlibx/devices/adc_scanner.h"
#include "avrlibx/io/gpio.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/timer.h"
#include "avrlibx/utils/op.h"

using namespace avrlibx;

PWM<PortF, 0> pwm_a;
PWM<PortF, 1> pwm_b;
ADCScanner<PortA, 0, 2, 3> adc_scanner;

ADC<PortA, 0> adc;

int main(void) {
  SysInit();
  pwm_a.Init(8);
  pwm_b.Init(8);
  
  adc_scanner.Init();
  while (1) {
    adc_scanner.Read();
    if (adc_scanner.index()) {
      pwm_b.Write(adc_scanner.value() >> 2);
    } else {
      pwm_a.Write(adc_scanner.value() >> 2);
    }
  }
}
