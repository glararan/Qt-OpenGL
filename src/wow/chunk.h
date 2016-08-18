#ifndef CHUNK_H
#define CHUNK_H

#include <QObject>
#include <QChar>
#include <QFile>

class Chunk
{
public:
    Chunk();
    explicit Chunk(const QString& cLetters, const int& cSize, const QByteArray& cData);
    explicit Chunk(QFile& file, int& offset);

    int getSize() const;
    int getOffset(const int offsetData) const;
    QByteArray getChunk() const;

    template<typename T>
    T& getStructure()
    {
        return *reinterpret_cast<T*>(data.data());
    }

    bool isEmpty() const;

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

#endif // CHUNK_H
