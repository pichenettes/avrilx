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

#ifndef AVRLIBX_IO_GPIO_H_
#define AVRLIBX_IO_GPIO_H_

#include <avr/io.h>

#include "avrlibx/avrlibx.h"

namespace avrlibx {

enum PortDirection {
  INPUT,
  OUTPUT
};

enum PortMode {
  PORT_MODE_TOTEM_POLE,
  PORT_MODE_BUS_KEEPER,
  PORT_MODE_PULL_UP,
  PORT_MODE_PULL_DOWN
};

#define WRAP_PORT(letter) \
struct Port ## letter { \
  static inline PORT_t& port_t() { return PORT ## letter; } \
  static inline void dir(uint8_t value) { PORT ## letter ## _DIR = value; } \
  static inline void dir_clr(uint8_t value) { PORT ## letter ## _DIRCLR = value; } \
  static inline void dir_set(uint8_t value) { PORT ## letter ## _DIRSET = value; } \
  static inline void out(uint8_t value) { PORT ## letter ## _OUT = value; } \
  static inline void out_set(uint8_t value) { PORT ## letter ## _OUTSET = value; } \
  static inline void out_clr(uint8_t value) { PORT ## letter ## _OUTCLR = value; } \
  static inline void out_tgl(uint8_t value) { PORT ## letter ## _OUTTGL = value; } \
  static inline uint8_t in() { return PORT ## letter ## _IN; } \
  static inline uint8_t dir() { return PORT ## letter ## _DIR; } \
  static inline uint8_t out() { return PORT ## letter ## _OUT; } \
};

WRAP_PORT(A)
WRAP_PORT(B)
WRAP_PORT(C)
WRAP_PORT(D)
WRAP_PORT(E)
#ifdef PORTF
  WRAP_PORT(F)
#endif

template<typename Port, uint8_t bit>
struct PinControl {
  static inline void set_control(uint8_t value) { }
};

#define SPECIALIZE_PIN_CONTROL(id) \
template<typename Port> \
struct PinControl<Port, id> { \
  static inline void set(uint8_t value) { \
    Port::port_t().PIN ## id ## CTRL = value; \
  } \
};

SPECIALIZE_PIN_CONTROL(0)
SPECIALIZE_PIN_CONTROL(1)
SPECIALIZE_PIN_CONTROL(2)
SPECIALIZE_PIN_CONTROL(3)
SPECIALIZE_PIN_CONTROL(4)
SPECIALIZE_PIN_CONTROL(5)
SPECIALIZE_PIN_CONTROL(6)
SPECIALIZE_PIN_CONTROL(7)

template<typename Port, uint8_t bit>
struct Gpio {
  static inline void set_direction(PortDirection direction) {
    if (direction == INPUT) {
      Port::dir_clr(_BV(bit));
    } else {
      Port::dir_set(_BV(bit));
    }
  }
  
  static inline void set_mode(PortMode mode) {
    if (mode == PORT_MODE_TOTEM_POLE) {
      PinControl<Port, bit>::set(PORT_OPC_TOTEM_gc);
    } else if (mode == PORT_MODE_BUS_KEEPER) {
      PinControl<Port, bit>::set(PORT_OPC_BUSKEEPER_gc);
    } else if (mode == PORT_MODE_PULL_UP) {
      PinControl<Port, bit>::set(PORT_OPC_PULLUP_gc);
    } else if (mode == PORT_MODE_PULL_DOWN) {
      PinControl<Port, bit>::set(PORT_OPC_PULLDOWN_gc);
    }
  }
  
  static inline void High() {
    Port::out_set(_BV(bit));
  }
  static inline void Low() {
    Port::out_clr(_BV(bit));
  }
  static inline void Toggle() {
    Port::out_tgl(_BV(bit));
  }
  
  static inline void set_value(uint8_t value) {
    if (value) {
      Port::out_set(_BV(bit));
    } else {
      Port::out_clr(_BV(bit));
    }
  }
  
  static inline uint8_t value() { 
    return Port::in() & _BV(bit) ? HIGH : LOW; 
  }
  
  static inline void Write(uint8_t value) { set_value(value); }
  static inline uint8_t Read() { return value(); }
};

struct DummyGpio {
  static inline void set_direction(PortDirection direction) { }
  static inline void set_mode(PortDirection direction) { }
  
  static inline void High() { }
  static inline void Low() { }
  static inline void Toggle() { }
  
  static inline void set_value(uint8_t value) { }
  static inline uint8_t value() { return 0; }
  static inline void Write(uint8_t value) { set_value(value); }
  static inline uint8_t Read() { return value(); }
};

template<typename Gpio>
struct Inverter {
  static inline void set_direction(PortDirection direction) {
    Gpio::set_direction(direction);
  }
  static inline void set_mode(PortDirection direction) {
    Gpio::set_mode(direction);
  }
  
  static inline void High() { Gpio::Low(); }
  static inline void Low() { Gpio::High(); }
  static inline void Toggle() { Gpio::Toggle(); }
  
  static inline void set_value(uint8_t value) { Gpio::set_value(!value); }
  static inline uint8_t value() { return !Gpio::value(); }
  static inline void Write(uint8_t value) { set_value(value); }
  static inline uint8_t Read() { return value(); }
};

}  // namespace avrlibx

#endif   // AVRLIBX_IO_GPIO_H_
