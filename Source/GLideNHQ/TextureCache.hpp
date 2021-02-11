/*
 * Rosalie's Texture Cache Tool - https://github.com/Rosalie241/TextureCacheTool
 *  Copyright (C) 2021 Rosalie Wanders <rosalie@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3.
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef TEXTURECACHE_HPP
#define TEXTURECACHE_HPP

#include <cstdint>

#include <QList>
#include <QImage>
#include <QString>
#include <QFile>
#include <QDataStream>

typedef struct
{
    uint64_t Checksum;
    QString ChecksumString;
	uint64_t Offset;
} Texture;

Q_DECLARE_METATYPE(Texture);

class TextureCache : QObject
{
    Q_OBJECT
    public:
        TextureCache(QString);
        ~TextureCache();
        
        QList<Texture> GetTextures(void);

        bool ReplaceTexture(Texture, QString);
        bool SaveTexture(Texture, QString);

        QImage GetImage(Texture);
    private:
        QList<Texture> textures;
        QString fileName;
        FILE* file = NULL;

        struct GHQTexInfo getRawTexInfo(Texture);

        bool readTexInfo(struct GHQTexInfo *, FILE *);
        bool writeTexInfo(struct GHQTexInfo *, FILE *);
        
        bool overWriteMappingOffset(uint64_t, uint64_t);

        void fillTextureList(void);
};

#endif // TEXTURECACHE_HPP
