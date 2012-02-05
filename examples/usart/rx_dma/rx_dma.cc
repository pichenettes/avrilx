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

#include "avrlibx/io/dma.h"
#include "avrlibx/io/gpio.h"
#include "avrlibx/io/usart.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/time.h"

using namespace avrlibx;

typedef Usart<PortC, 1, 31250, RX_POLLED, TX_POLLED> UsartC;

Gpio<PortF, 0> led_a;
DMARxBuffer<UsartC, 0, 64> dma;
UsartC usart;

int main(void) {
  SysInit();
  led_a.set_direction(OUTPUT);
  usart.Init();
  dma.Init();
  dma.Start();
  while (1) {
    if (dma.readable()) {
      if (dma.ImmediateRead() == 0xf8) {
        led_a.Toggle();
      }
    }
  }
}
