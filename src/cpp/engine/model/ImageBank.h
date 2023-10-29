/* Copyright 2023 Zmallwood
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#pragma once

namespace nar {

   struct FileData {
      const long data_length;
      const void *data;
      const void *file_handle;
   };

   struct RawImageData {
      const int width;
      const int height;
      const int size;
      const GLenum gl_color_format;
      const void *data;
   };

   struct DataHandle {
      const png_byte *data;
      const png_size_t size;
   };

   struct ReadDataHandle {
      const DataHandle data;
      png_size_t offset;
   };

   struct PngInfo {
      const png_uint_32 width;
      const png_uint_32 height;
      const int color_type;
   };

   /** Loads and provides images used in the rendering.
    */
   class ImageBank : public Singleton<ImageBank> {
     public:
      void LoadImages();
      GLuint GetImage(std::string_view image_name);

     private:
      void LoadSingleImage(std::string_view file_name);
      GLuint LoadPNGAssetIntoTexture(const char *relative_path);
      FileData GetAssetData(const char *relative_path);
      void ReleaseAssetData(const FileData *file_data);
      RawImageData GetRawImageDataFromPNG(const void *png_data, const int png_data_size);
      void ReleaseRawImageData(const RawImageData *data);
      GLuint LoadTexture(
          const GLsizei width, const GLsizei height, const GLenum type, const GLvoid *pixels);
      PngInfo ReadAndUpdateInfo(const png_structp png_ptr, const png_infop info_ptr);
      DataHandle ReadEntirePNGImage(
          const png_structp png_ptr, const png_infop info_ptr, const png_uint_32 height);
      GLenum GetGLColorFormat(const int png_color_format);
      static void
      ReadPNGDataCallback(png_structp png_ptr, png_byte *raw_data, png_size_t read_length);

      std::map<std::string_view, GLuint> images_;
   };
}