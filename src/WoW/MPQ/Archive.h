#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <QObject>

#include <Core/AsyncWorker.h>

#include "ArchiveFile.h"

typedef void* HANDLE;

namespace WoW
{
    class ArchiveManager;

    class Archive : public AsyncWorker
    {
        Q_OBJECT

    public:
        explicit Archive(const QString& file, bool processListFile, bool create = false, QObject* parent = Q_NULLPTR);
        ~Archive();

        void addFile(ArchiveFile* file, QString mpqPath);
        bool hasFile(const QString& fileName) const;
        bool readFile(const QString& fileName, size_t* size, char** buffer) const;

        // todo: implement getADT, getWDL, getWDT

        void process() Q_DECL_OVERRIDE;
        void save();

        const QStringList& getListFile() const { return listFile; }

    private:
        HANDLE handler = Q_NULLPTR;

        QStringList listFile;

        friend class ArchiveManager;
    };
}

#endif // ARCHIVE_H
