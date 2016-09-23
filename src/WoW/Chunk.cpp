#include "Chunk.h"

namespace WoW
{
    BaseChunk::BaseChunk()
    {
    }

    BaseChunk::BaseChunk(const QString& cLetters, const int& cSize, const QByteArray& cData)
    : letters(cLetters)
    , size(cSize)
    , data(cData)
    {
    }

    BaseChunk::BaseChunk(const QString& cLetters, const int& cSize)
    : letters(cLetters)
    , size(cSize)
    {
    }

    BaseChunk::BaseChunk(QFile& file, int& offset)
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

    QByteArray BaseChunk::getChunk() const
    {
        QByteArray array;

        for(const QChar& character : letters)
            array.append(character);

        array.append(QByteArray::fromRawData((const char*)&size, sizeof(size)));
        array.append(data); // some annoying bug here, somehow data is modified even without touching them

        return array;
    }
}
