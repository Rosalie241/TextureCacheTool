#ifndef CONVERT_FILE_H
#define CONVERT_FILE_H

#include <stdint.h>

struct GHQTexInfo 
{
    int32_t width;
    int32_t height;
    uint32_t format;
    uint16_t texture_format;
    uint16_t pixel_type;
    uint8_t is_hires_tex;
    uint32_t data_size;
    uint8_t *data;
};

#define TEXTUREFILE_MAGIC "TextureFile_1_0"

struct TextureFile
{
    char Magic[sizeof(TEXTUREFILE_MAGIC)];
    struct GHQTexInfo Info;
};

#ifdef __cplusplus
extern "C"
{
#endif

extern int prepare_file(struct GHQTexInfo *file);

#ifdef __cplusplus
}
#endif 

#endif // CONVERT_FILE_H
