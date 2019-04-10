// Copyright 2011 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
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
//
// -----------------------------------------------------------------------------
//
// Important: All buffer sizes are expected to be less than 256! (fit in 8
// bits), and must be powers of 2.

#ifndef AVRLIBX_AVRLIBX_H_
#define AVRLIBX_AVRLIBX_H_

#include <inttypes.h>

#ifndef NULL
#define NULL 0
#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

template<bool b>
inline void StaticAssertImplementation() {
	char static_assert_size_mismatch[b] = { 0 };
}
 
#define STATIC_ASSERT(expression) StaticAssertImplementation<(expression)>()

typedef union {
  uint16_t value;
  uint8_t bytes[2];
} Word;

typedef union {
  uint32_t value;
  uint16_t words[2];
  uint8_t bytes[4];
} LongWord;

struct uint24_t {
  uint16_t integral;
  uint8_t fractional;
};

struct uint24c_t {
  uint8_t carry;
  uint16_t integral;
  uint8_t fractional;
};

template<uint32_t a, uint32_t b, uint32_t c, uint32_t d>
struct FourCC {
  static const uint32_t value = (((((d << 8) | c) << 8) | b) << 8) | a;
};

namespace avrlibx {

template<uint8_t size>
struct DataTypeForSize {
  typedef uint16_t Type;
};

template<> struct DataTypeForSize<1> { typedef uint8_t Type; };
template<> struct DataTypeForSize<2> { typedef uint8_t Type; };
template<> struct DataTypeForSize<3> { typedef uint8_t Type; };
template<> struct DataTypeForSize<4> { typedef uint8_t Type; };
template<> struct DataTypeForSize<5> { typedef uint8_t Type; };
template<> struct DataTypeForSize<6> { typedef uint8_t Type; };
template<> struct DataTypeForSize<7> { typedef uint8_t Type; };
template<> struct DataTypeForSize<8> { typedef uint8_t Type; };

enum DataOrder {
  MSB_FIRST = 0,
  LSB_FIRST = 1
};

enum DigitalValue {
  LOW = 0,
  HIGH = 1
};

// Some classes (SPI, shift register) have a notion of communication session -
// Begin is called, several R/W are done, and then End is called to pull high
// a chip select or latch line. This template ensures that any path leaving a
// block of code will release the resource.
template<typename T>
class scoped_resource {
 public:
  scoped_resource() {
    T::Begin();
  }
  
  ~scoped_resource() {
    T::End();
  }
};

}  // namespace avrlibx

#endif   // AVRLIBX_AVRLIBX_H_
