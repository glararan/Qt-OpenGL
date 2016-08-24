#include "Chunk.h"

#define lettersAndSize 8

namespace WoW
{
    Chunk::Chunk()
    {
    }

    Chunk::Chunk(const QString &cLetters, const int& cSize, const QByteArray& cData)
    : letters(cLetters)
    , size(cSize)
    , data(cData)
    {
    }

    Chunk::Chunk(QFile& file, int& offset)
    {
        file.seek(offset);

        letters = QString(file.read(4));

        offset += 4;

        file.seek(offset);

        size = file.read(4).toInt();

        offset += 4;

        file.seek(offset);

        data = file.read(size);

        offset += lettersAndSize + data.size();
    }

    QByteArray Chunk::getChunk() const
    {
        QByteArray array;

        for(const QChar& character : letters)
            array.append(character);

        /*array.append(size & 0xff);
        array.append((size >> 8)  & 0xff);
        array.append((size >> 16) & 0xff);
        array.append((size >> 24) & 0xff);*/

        array.append(QByteArray::number(size));
        array.append(data);

        return array;

        /*QVector<QChar> chunk;

        for(const QChar& character : letters)
            chunk.append(character);

        QVector<QChar> sizeVector;
        sizeVector.append(size & 0xff);
        sizeVector.append((size >> 8)  & 0xff);
        sizeVector.append((size >> 16) & 0xff);
        sizeVector.append((size >> 24) & 0xff);

        chunk.append(sizeVector);
        chunk.append(data);

        return chunk;*/
    }
}
