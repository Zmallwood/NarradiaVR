/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar
{
   struct FileData {
      const long data_length;
      const void *data;
      const void *file_handle;
   };
}