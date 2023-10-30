#pragma once

namespace nar
{
   struct FileData {
      const long data_length;
      const void *data;
      const void *file_handle;
   };
}