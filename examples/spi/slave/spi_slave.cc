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

#include "avrlibx/io/parallel_io.h"
#include "avrlibx/io/spi.h"
#include "avrlibx/system/init.h"

using namespace avrlibx;

SPISlave<PortD, MSB_FIRST> spi;
ParallelPort<PortF, 0, 1> leds;

int main(void) {
  SysInit();
  spi.Init();
  leds.set_direction(OUTPUT);
  while (1) {
    leds.set_value(spi.Read());
  }
}
