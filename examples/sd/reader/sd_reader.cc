// Copyright 2012 Olivier Gillet.
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

#include "avrlibx/devices/sd_card.h"
#include "avrlibx/filesystem/fat_file_reader.h"
#include "avrlibx/io/gpio.h"
#include "avrlibx/io/spi.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/time.h"

using namespace avrlibx;

Gpio<PortF, 0> led_a;
Gpio<PortF, 1> led_b;

typedef SPIMaster<
    PortD,
    Gpio<PortD, 4>,
    MSB_FIRST,
    SPI_PRESCALER_CLK_4,
    true> SpiInterface;
typedef SdCard<SpiInterface> SdCardInterface;
FATFileReader<SdCardInterface> reader;

void LedsPattern(uint8_t error) {
  led_a.High();
  led_b.set_value(error);
  for (uint8_t i = 0; i < 10; ++i) {
    ConstantDelay(100);
    led_a.Toggle();
    led_b.Toggle();
  }
}

int main(void) {
  SysInit();
  led_a.set_direction(OUTPUT);
  led_b.set_direction(OUTPUT);
  
  FsHandle h;
  uint8_t read_byte;
  // If the file TEST.TXT is present in the card and its first byte is '7',
  // Alternating blink pattern on the two LEDs.
  if (reader.Init() == FFR_OK && reader.Open("TEST    TXT", &h) == FFR_OK &&
      reader.Read(&h, 1, &read_byte) == 1 && read_byte == '7') {
    LedsPattern(0);
  } else {
    // Otherwise, the LEDs blink together.
    LedsPattern(1);
  }
  
  while (1);
}
