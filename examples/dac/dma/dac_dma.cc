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
#include "avrlibx/io/dma.h"
#include "avrlibx/io/gpio.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/timer.h"
#include "avrlibx/system/time.h"

using namespace avrlibx;

Timer<PortF, 0> timer;
DAC<PortB> dac;
DMATxBuffer<0, 128, DAC<PortB>::Channel<0>, Timer<PortF, 0> > dma;

static const uint8_t kRenderBlockSize = 40;

int main(void) {
  SysInit();

  dac.Init();
  dma.Init();

  timer.set_prescaler(TIMER_PRESCALER_CLK);
  timer.set_period(800);
  timer.set_mode(TIMER_MODE_NORMAL);

  dma.Start();

  uint16_t counter = 0;
  while (1) {
    if (dma.writable() >= kRenderBlockSize) {
      for (uint8_t i = 0; i < kRenderBlockSize; ++i) {
        dma.Overwrite(counter++ & 0xfff);
      }
    }
  }
}
