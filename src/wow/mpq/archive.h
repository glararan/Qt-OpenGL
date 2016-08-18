#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <QObject>

#include "ArchiveFile.h"

typedef void* HANDLE;

class ArchiveManager;

class Archive : public QObject
{
    Q_OBJECT

public:
    explicit Archive(const QString& file, bool processListFile, bool create = false, QObject* parent = Q_NULLPTR);
    ~Archive();

    void addFile(ArchiveFile* file, QString mpqPath);
    ArchiveFile* getFile(const QString& mpqPath);
    bool hasFile(const QString& fileName) const;
    bool readFile(const QString& fileName, size_t* size, char** buffer) const;

    void finishLoading();
    void save();

    const bool isLoadingFinished() const { return finished; }

    const QStringList& getListFile() const { return listFile; }

private:
    HANDLE handler = Q_NULLPTR;

    QStringList listFile;

    bool finished = false;

    friend class ArchiveManager;

signals:
    void loadingFinished();
};

#endif // ARCHIVE_H
