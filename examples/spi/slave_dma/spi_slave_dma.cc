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
#include "avrlibx/io/parallel_io.h"
#include "avrlibx/io/spi.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/time.h"

using namespace avrlibx;

typedef SPISlave<PortD, MSB_FIRST> Spi;
DMARxBuffer<Spi, 0, 64> dma;
Spi spi;
ParallelPort<PortF, 0, 1> leds;

int main(void) {
  SysInit();
  leds.set_direction(OUTPUT);
  spi.Init();
  dma.Init();
  dma.Start();
  while (1) {
    if (dma.readable() >= 8) {
      for (uint8_t i = 0; i < 8; ++i) {
        leds.set_value(dma.ImmediateRead());
        ConstantDelay(200);
      }
    }
  }
}

