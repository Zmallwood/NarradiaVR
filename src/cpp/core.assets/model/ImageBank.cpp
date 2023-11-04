/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "ImageBank.h"
#include "../../core/model/AppManager.h"

namespace nar {
    void ImageBank::LoadImages() {
        auto asset_manager = AppManager::Get()->app()->activity->assetManager;
        AAssetDir *dir = AAssetManager_openDir(asset_manager, "");
        const char *file_name = AAssetDir_getNextFileName(dir);

        while (file_name != nullptr) {
            if (Util::GetFileExtension(file_name) == "png") {
                std::string_view file_name_with_ext = file_name;
                LoadSingleImage(file_name);
            }

            file_name = AAssetDir_getNextFileName(dir);
        }
    }

    GLuint ImageBank::GetImage(std::string_view image_name) {
        if (images_.count(image_name))
            return images_.at(image_name);

        return 0;
    }

    void ImageBank::LoadSingleImage(std::string_view file_name) {
        std::string_view file_name_no_ext = Util::GetFileNameNoExt(file_name);
        GLuint texture = LoadPNGAssetIntoTexture(file_name.data());
        images_.insert({file_name_no_ext, texture});
    }

    GLuint ImageBank::LoadPNGAssetIntoTexture(const char *relative_path) {
        const FileData png_file = GetAssetData(relative_path);
        const RawImageData raw_image_data =
            GetRawImageDataFromPNG(png_file.data, png_file.data_length);
        const GLuint texture_object_id = LoadTexture(
            raw_image_data.width, raw_image_data.height, raw_image_data.gl_color_format,
            raw_image_data.data);

        ReleaseRawImageData(&raw_image_data);
        ReleaseAssetData(&png_file);

        return texture_object_id;
    }

    FileData ImageBank::GetAssetData(const char *relative_path) {
        auto asset_manager = AppManager::Get()->app()->activity->assetManager;
        AAsset *asset = AAssetManager_open(asset_manager, relative_path, AASSET_MODE_STREAMING);

        return (FileData){AAsset_getLength(asset), AAsset_getBuffer(asset), asset};
    }

    void ImageBank::ReleaseAssetData(const FileData *file_data) {
        AAsset_close((AAsset *)file_data->file_handle);
    }

    RawImageData ImageBank::GetRawImageDataFromPNG(const void *png_data, const int png_data_size) {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop info_ptr = png_create_info_struct(png_ptr);

        ReadDataHandle png_data_handle =
            (ReadDataHandle){{(const png_byte *)png_data, (png_size_t)png_data_size}, 0};
        png_set_read_fn(png_ptr, &png_data_handle, ReadPNGDataCallback);

        if (setjmp(png_jmpbuf(png_ptr))) {
            __android_log_print(ANDROID_LOG_INFO, "Narradia", "Error reading png file");
        }

        const PngInfo png_info = ReadAndUpdateInfo(png_ptr, info_ptr);
        const DataHandle raw_image = ReadEntirePNGImage(png_ptr, info_ptr, png_info.height);

        png_read_end(png_ptr, info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        return (RawImageData){
            (int)png_info.width, (int)png_info.height, (int)raw_image.size,
            GetGLColorFormat(png_info.color_type), raw_image.data};
    }

    void ImageBank::ReleaseRawImageData(const RawImageData *data) {
        free((void *)data->data);
    }

    GLuint ImageBank::LoadTexture(
        const GLsizei width, const GLsizei height, const GLenum type, const GLvoid *pixels) {
        GLuint texture_object_id;
        glGenTextures(1, &texture_object_id);

        glBindTexture(GL_TEXTURE_2D, texture_object_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        return texture_object_id;
    }

    void ImageBank::ReadPNGDataCallback(
        png_structp png_ptr, png_byte *raw_data, png_size_t read_length) {
        ReadDataHandle *handle = (ReadDataHandle *)png_get_io_ptr(png_ptr);
        const png_byte *png_src = handle->data.data + handle->offset;

        memcpy(raw_data, png_src, read_length);
        handle->offset += read_length;
    }

    PngInfo ImageBank::ReadAndUpdateInfo(const png_structp png_ptr, const png_infop info_ptr) {
        png_uint_32 width, height;
        int bit_depth, color_type;

        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

        // Convert transparency to full alpha
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
            png_set_tRNS_to_alpha(png_ptr);

        // Convert grayscale, if needed.
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
            png_set_expand_gray_1_2_4_to_8(png_ptr);

        // Convert paletted images, if needed.
        if (color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_palette_to_rgb(png_ptr);

        // Add alpha channel, if there is none.
        // Rationale: GL_RGBA is faster than GL_RGB on many GPUs)
        if (color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB)
            png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

        // Ensure 8-bit packing
        if (bit_depth < 8)
            png_set_packing(png_ptr);
        else if (bit_depth == 16)
            png_set_scale_16(png_ptr);

        png_read_update_info(png_ptr, info_ptr);

        // Read the new color type after updates have been made.
        color_type = png_get_color_type(png_ptr, info_ptr);

        return (PngInfo){width, height, color_type};
    }

    DataHandle ImageBank::ReadEntirePNGImage(
        const png_structp png_ptr, const png_infop info_ptr, const png_uint_32 height) {
        const png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
        const int data_length = row_size * height;

        png_byte *raw_image = (png_byte *)malloc(data_length);

        png_byte *row_ptrs[height];

        png_uint_32 i;
        for (i = 0; i < height; i++) {
            row_ptrs[i] = raw_image + i * row_size;
        }

        png_read_image(png_ptr, &row_ptrs[0]);

        return (DataHandle){raw_image, (png_size_t)data_length};
    }

    GLenum ImageBank::GetGLColorFormat(const int png_color_format) {
        switch (png_color_format) {
        case PNG_COLOR_TYPE_GRAY:
            return GL_LUMINANCE;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            return GL_RGBA;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            return GL_LUMINANCE_ALPHA;
        }

        return 0;
    }
}
