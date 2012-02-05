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

#include "avrlibx/devices/twi_eeprom.h"
#include "avrlibx/io/usart.h"
#include "avrlibx/system/init.h"
#include "avrlibx/system/time.h"
#include "avrlibx/utils/output_stream.h"

using namespace avrlibx;

// Debug text output.
typedef Usart<PortD, 0, 2400, RX_DISABLED, TX_POLLED> DebugSerial;
OutputStream<DebugSerial> debug_output;
DebugSerial debug_serial;

// Eeprom
TwiEeprom<TwiMaster<PortC, 100000, 64, 64>, 8192 > eeprom;

TICK_ISR;

int main(void) {
  SysInit();
  SetupTickTimer();
  eeprom.Init();
  debug_serial.Init();
  Gpio<PortC, 0>::set_mode(PORT_MODE_PULL_UP);
  Gpio<PortC, 1>::set_mode(PORT_MODE_PULL_UP);
  
  uint8_t buffer[32];
  for (uint8_t i = 0; i < 32; ++i) {
    buffer[i] = i + 100;
  }

  eeprom.WriteWithinBlock(0x20, buffer, 32);
  debug_output << "Written" << endl;
  Delay(5);  // Write/read cycles delay.

  debug_output << "Reading" << endl;
  eeprom.SetAddress(0x20);
  for (uint8_t i = 0; i < 32; ++i) {
    debug_output << int(eeprom.Read()) << endl;
  }
  debug_output << "Done" << endl;

  eeprom.SetAddress(0x20);
  uint8_t buffer2[32];
  eeprom.Read(0x20, 32, buffer2);
  for (uint8_t i = 0; i < 32; ++i) {
    debug_output << int(buffer2[i]) << endl;
  }
  debug_output << "Done" << endl;
  while (1);
}
