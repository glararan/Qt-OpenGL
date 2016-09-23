#ifndef CHUNK_H
#define CHUNK_H

#include <QObject>
#include <QChar>
#include <QFile>

#include "ChunkHeaders.h"

#define lettersAndSize 8

namespace WoW
{
    class BaseChunk
    {
    public:
        BaseChunk();
        explicit BaseChunk(const QString& cLetters, const int& cSize, const QByteArray& cData);
        explicit BaseChunk(const QString& cLetters, const int& cSize);
        explicit BaseChunk(QFile& file, int& offset);

        int getSize() const                       { return data.size(); }
        int getOffset(const int offsetData) const { return offsetData; }
        QByteArray getData() const                { return data; }
        QByteArray getChunk() const;

        template<class T>
        T& getStructure() { return *reinterpret_cast<T*>(data.data()); }

        bool isEmpty() const { return letters == "NONE" && size == 0 && data.size() == 0; }

        template<class T>
        void setStructure(T& value)
        {
            data = QByteArray::fromRawData(reinterpret_cast<char*>(&value), sizeof(T));//QByteArray(reinterpret_cast<const char*>(&value));
        }

    protected:
        QString letters = "NONE";
        QByteArray data;

        int size = 0;
    };

    template<class T>
    class Chunk : public BaseChunk
    {
    public:
        Chunk()
        : BaseChunk()
        {
        }

        explicit Chunk(const QString& cLetters, T& cData)
        : BaseChunk(cLetters, sizeof(cData))
        {
            setStructure(cData);
        }

        explicit Chunk(const QString& cLetters, const int& cSize, const QByteArray& cData)
        : BaseChunk(cLetters, cSize, cData)
        {
        }

        explicit Chunk(const QString& cLetters, const QByteArray& cData)
        : BaseChunk(cLetters, sizeof(T), cData)
        {
            tData = {};

            setStructure(tData);
        }

        explicit Chunk(const QString& cLetters)
        : BaseChunk(cLetters, sizeof(T))
        {
            tData = {};

            setStructure(tData);
        }

        explicit Chunk(QFile& file, int& offset)
        : BaseChunk(file, offset)
        {
        }

        T& getStructure() { return tData; }
        QByteArray getChunk() const
        {
            QByteArray array;

            for(const QChar& character : letters)
                array.append(character);

            array.append(QByteArray::fromRawData((const char*)&size, sizeof(size)));
            array.append(QByteArray::fromRawData((const char*)&tData, sizeof(T)));

            return array;
        }

        void setStructure(T& value)
        {
            tData = value;
            data  = QByteArray::fromRawData((const char*)&value, sizeof(T)); //QByteArray::fromRawData(reinterpret_cast<char*>(&value), sizeof(T));//QByteArray(reinterpret_cast<const char*>(&value));
        }

    protected:
        T tData;
    };
}

#endif // CHUNK_H
