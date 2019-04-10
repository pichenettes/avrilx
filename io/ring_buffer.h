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

#ifndef AVRLIBX_IO_RING_BUFFER_H_
#define AVRLIBX_IO_RING_BUFFER_H_

#include "avrlibx/avrlibx.h"

namespace avrlibx {

template<typename Owner>
class RingBuffer {
 public:
  typedef typename Owner::Value Value;
  enum {
    size = Owner::buffer_size,
    data_size = Owner::data_size
  };
  
  RingBuffer() { }
  
  static inline uint8_t capacity() { return size; }
  static inline void Write(Value v) {
    while (!writable());
    Overwrite(v);
  }
  static inline uint8_t writable() {
    return (read_ptr_ - write_ptr_ - 1) & (size - 1);
  }
  static inline uint8_t NonBlockingWrite(Value v) {
    if (writable()) {
      Overwrite(v);
      return 1;
    } else {
      return 0;
    }
  }
  static inline void Overwrite(Value v) {
    uint8_t w = write_ptr_;
    buffer_[w] = v;
    write_ptr_ = (w + 1) & (size - 1);
  }
  static void Overwrite2(Value v1, Value v2) {
    uint8_t w = write_ptr_;
    buffer_[w] = v1;
    buffer_[w + 1] = v2;
    write_ptr_ = (w + 2) & (size - 1);
  }
  
  static inline uint8_t Requested() { return 0; }
  static inline Value Read() {
    while (!readable());
    return ImmediateRead();
  }
  static inline uint8_t readable() {
    return (write_ptr_ - read_ptr_) & (size - 1);
  }
  static inline int16_t NonBlockingRead() {
    if (readable()) {
      return ImmediateRead();
    } else {
      return -1;
    }
  }
  static inline Value ImmediateRead() {
    uint8_t r = read_ptr_;
    Value result = buffer_[r];
    read_ptr_ = (r + 1) & (size - 1);
    return result;
  }
  static inline void Flush() {
    write_ptr_ = read_ptr_;
  }
 private:
  static Value buffer_[size];
  static volatile uint8_t read_ptr_;
  static volatile uint8_t write_ptr_;

  DISALLOW_COPY_AND_ASSIGN(RingBuffer);
};

// Static variables created for each buffer.
template<typename T> volatile uint8_t RingBuffer<T>::read_ptr_ = 0;
template<typename T> volatile uint8_t RingBuffer<T>::write_ptr_ = 0;
template<typename T> typename T::Value RingBuffer<T>::buffer_[];

}  // namespace avrlibx

#endif   // AVRLIBX_IO_RING_BUFFER_H_
