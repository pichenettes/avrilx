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
// FatFS wrappers.

#ifndef AVRLIBX_FILESYSTEM_DIRECTORY_H_
#define AVRLIBX_FILESYSTEM_DIRECTORY_H_

#include <string.h>

#include "avrlibx/avrlibx.h"

#include "avrlibx/filesystem/filesystem.h"

namespace avrlibx {

class Directory {
 public:
  Directory() { }

  FilesystemStatus Open(const char* directory_name) {
    return Open(directory_name, 0);
  }
  FilesystemStatus Open(const char* directory_name, uint16_t retry_timeout);
  FilesystemStatus Rewind();
  FilesystemStatus Next();
  
  inline uint8_t done() const { return f_.file_info.fname[0] == '\0'; }
  
  inline const FileInfo& entry() const { return f_; }
  
 private:
  DIR d_;
  FileInfo f_;
  
  DISALLOW_COPY_AND_ASSIGN(Directory);
};

}  // namespace avrlibx

#endif   // AVRLIBX_FILESYSTEM_DIRECTORY_H_
