#ifndef CHUNK_H
#define CHUNK_H

#include <QObject>
#include <QChar>
#include <QFile>

namespace WoW
{
    class Chunk
    {
    public:
        Chunk();
        explicit Chunk(const QString& cLetters, const int& cSize, const QByteArray& cData);
        explicit Chunk(QFile& file, int& offset);

        int getSize() const                       { return data.size(); }
        int getOffset(const int offsetData) const { return offsetData; }
        QByteArray getData() const                { return data; }
        QByteArray getChunk() const;

        template<typename T>
        T& getStructure()
        {
            return *reinterpret_cast<T*>(data.data());
        }

        bool isEmpty() const { return letters == "NONE" && size == 0 && data.size() == 0; }

        template<typename T>
        void setStructure(T& value)
        {
            data = QByteArray::fromRawData(reinterpret_cast<char*>(&value), sizeof(T));//QByteArray(reinterpret_cast<const char*>(&value));
        }

    protected:
        QString letters = "NONE";
        QByteArray data;

        int size = 0;
    };
}

#endif // CHUNK_H
