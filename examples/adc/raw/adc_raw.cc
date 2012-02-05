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

#include "avrlibx/io/adc.h"
#include "avrlibx/io/gpio.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/timer.h"
#include "avrlibx/utils/op.h"

using namespace avrlibx;

PWM<PortF, 0> pwm_a;
PWM<PortF, 1> pwm_b;
ADC<PortA, 0> adc;

int main(void) {
  SysInit();
  pwm_a.Init(8);
  pwm_b.Init(8);
  
  adc.Init();
  while (1) {
    int16_t result = 0;
    
    adc.StartConversion(2);
    result = Clip(adc.Read(), 0, 32767) >> 8;
    pwm_a.Write(result << 1);
    
    adc.StartConversion(3);
    result = Clip(adc.Read(), 0, 32767) >> 8;
    pwm_b.Write(result << 1);
  }
}
