#ifndef ARCHIVEFILE_H
#define ARCHIVEFILE_H

#include <QObject>

namespace WoW
{
    class Archive;

    class ArchiveFile : public QObject
    {
        Q_OBJECT

    public:
        explicit ArchiveFile(const QString& file, const bool& create = false, QObject* parent = Q_NULLPTR);
        ~ArchiveFile();

        size_t getSize() const     { return size; }
        size_t getPosition() const { return pointer; }
        char*  getBuffer() const   { return buffer; }
        char*  getPointer() const  { return buffer + pointer; }

        void close();

        bool isOnDisk() const { return onDisk; }
        bool isEof() const    { return eof; }

        void setBuffer(char* value, size_t length);

        void save();
        void saveToMPQ(Archive* archive, const QString& mpqPath = Q_NULLPTR);

        static bool exists(const QString& file);
        static void diskSearchPath(const QString& path);

    private:
        Q_DISABLE_COPY(ArchiveFile)

        bool eof       = true; // end of file
        char* buffer   = Q_NULLPTR;
        size_t pointer = 0;
        size_t size    = 0;

        bool onDisk = false;

        QString fileName;

        friend class Archive;
    };
}

#endif // ARCHIVEFILE_H
