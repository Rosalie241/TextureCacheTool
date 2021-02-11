/*
 * Rosalie's Texture Cache Tool - https://github.com/Rosalie241/TextureCacheTool
 *  Copyright (C) 2021 Rosalie Wanders <rosalie@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3.
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include "TextureCache.hpp"
#include "convert_file.h"

#include "stdio.h"

TextureCache::TextureCache(QString file)
{
    this->fileName = file;
    this->fillTextureList();
}

TextureCache::~TextureCache(void)
{
    fclose(file);
}

QList<Texture> TextureCache::GetTextures(void)
{
    return textures;
}

bool TextureCache::ReplaceTexture(Texture texture, QString fileName)
{
    std::string fileNameStr = fileName.toStdString();
    struct TextureFile textureFile;

    FILE* inFile = fopen(fileNameStr.c_str(), "r");
    if (inFile == NULL)
    {
        return false;
    }

    /* Verify Magic */
    fread(&textureFile.Magic, sizeof(textureFile.Magic), 1, inFile);
    if (strcmp(textureFile.Magic, TEXTUREFILE_MAGIC) != 0)
    {
        fclose(inFile);
        return false;
    }

    if (!readTexInfo(&textureFile.Info, inFile))
    {
        return false;
    }
   
    struct GHQTexInfo info = getRawTexInfo(texture);

    if (textureFile.Info.data_size > info.data_size)
    {
        fseek(file, 0, SEEK_END);
        int64_t newOffset = ftell(file);
        if (!overWriteMappingOffset(texture.Checksum, newOffset))
        {
            free(textureFile.Info.data);
            fclose(inFile);
            return false;
        }
        fseek(file, newOffset, SEEK_SET);
    }
    else
    {
        fseek(file, texture.Offset, SEEK_SET);
    }

    writeTexInfo(&textureFile.Info, file);

    free(textureFile.Info.data);
    fclose(inFile);

    return true;
}

bool TextureCache::SaveTexture(Texture texture, QString fileName)
{
    std::string fileNameStr = fileName.toStdString();
    struct TextureFile textureFile;

    FILE* outFile = fopen(fileNameStr.c_str(), "w");
    if (outFile == NULL)
    {
        return false;
    }

    strcpy(textureFile.Magic, TEXTUREFILE_MAGIC);
    textureFile.Info = getRawTexInfo(texture);

    fwrite(&textureFile.Magic, sizeof(textureFile.Magic), 1, outFile);
    writeTexInfo(&textureFile.Info, outFile);

    fflush(outFile);
    fclose(outFile);

    free(textureFile.Info.data);

    return true;
}

QImage TextureCache::GetImage(Texture texture)
{
    QImage image;   

    struct GHQTexInfo info;
    
    info = getRawTexInfo(texture);

    if (prepare_file(&info) != 0)
    {
        return image;
    }

    image.loadFromData(QByteArray((char *)info.data, info.data_size));

    free(info.data);

    return image;
}

struct GHQTexInfo TextureCache::getRawTexInfo(Texture texture)
{
    struct GHQTexInfo info = {0};

    fseek(file, texture.Offset, SEEK_SET);
    readTexInfo(&info, file);

    return info;
}

bool TextureCache::readTexInfo(struct GHQTexInfo *info, FILE *file)
{
    fread(&info->width, sizeof(info->width), 1, file);
    fread(&info->height, sizeof(info->height), 1, file);
    fread(&info->format, sizeof(info->format), 1, file);
    fread(&info->texture_format, sizeof(info->texture_format), 1, file);
    fread(&info->pixel_type, sizeof(info->pixel_type), 1, file);
    fread(&info->is_hires_tex, sizeof(info->is_hires_tex), 1, file);
    fread(&info->data_size, sizeof(info->data_size), 1, file);
    info->data = (uint8_t*)malloc(info->data_size);
    fread(info->data, 1, info->data_size, file);
    return true;
}

bool TextureCache::writeTexInfo(struct GHQTexInfo *info, FILE *file)
{
    fwrite(&info->width, sizeof(info->width), 1, file);
    fwrite(&info->height, sizeof(info->height), 1, file);
    fwrite(&info->format, sizeof(info->format), 1, file);
    fwrite(&info->texture_format, sizeof(info->texture_format), 1, file);
    fwrite(&info->pixel_type, sizeof(info->pixel_type), 1, file);
    fwrite(&info->is_hires_tex, sizeof(info->is_hires_tex), 1, file);
    fwrite(&info->data_size, sizeof(info->data_size), 1, file);
    fwrite(info->data, info->data_size, 1, file);

    fflush(file);
    return true;
}

bool TextureCache::overWriteMappingOffset(uint64_t checksum, uint64_t offset)
{
    bool ret = false;

    fseek(file, 0, SEEK_SET);

    int config = 0;
    fread(&config, sizeof(config), 1, file);

    uint64_t map_offset = 0;
    fread(&map_offset, sizeof(map_offset), 1, file);
    fseek(file, map_offset, SEEK_SET);

    int32_t map_size = 0;
    fread(&map_size, sizeof(map_size), 1, file);

    for (int i = 0; i < map_size; i++)
    {
        uint64_t t_checksum, t_offset;
        
        fread(&t_checksum, sizeof(t_checksum), 1, file);

        if (t_checksum == checksum)
        {
            fwrite(&offset, sizeof(offset), 1, file);
            ret = true;
        }
        else
        {
            fread(&t_offset, sizeof(t_offset), 1, file);
        }
    }

    fillTextureList();
    return ret;
}

void TextureCache::fillTextureList(void)
{
    if (file != NULL)
    {
        fclose(file);
    }

    file = fopen(fileName.toStdString().c_str(), "r+");
    if (file == NULL)
    {
        return;
    }

    textures.clear();

    int config = 0;
    fread(&config, sizeof(config), 1, file);

    uint64_t map_offset = 0;
    fread(&map_offset, sizeof(map_offset), 1, file);
    fseek(file, map_offset, SEEK_SET);
    
    int32_t map_size = 0;
    fread(&map_size, sizeof(map_size), 1, file);

    for (int i = 0; i < map_size; i++)
    {
        Texture t;
        
        fread(&t.Checksum, sizeof(t.Checksum), 1, file);
        fread(&t.Offset, sizeof(t.Offset), 1, file);

        char buf[32];

        uint32_t palchksum = (t.Checksum >> 32);
        uint32_t checksum = (t.Checksum & 0xffffffff);

        snprintf(buf, sizeof(buf), "%08X %08X", checksum, palchksum);

        t.ChecksumString = QString(buf);

        textures.append(t);
    }
}
