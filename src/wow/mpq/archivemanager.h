#ifndef ARCHIVEMANAGER_H
#define ARCHIVEMANAGER_H

#include <QObject>
#include <QMutex>

#include "Archive.h"

class ArchiveManager : public QObject
{
    Q_OBJECT

public:
    explicit ArchiveManager(QObject* parent = Q_NULLPTR);
    ~ArchiveManager();

    Archive* create(const QString& file, bool processListFile = false);
    Archive* load(const QString& file, bool processListFile = false);

    void unload(const QString& file);
    void unloadAll();

    bool exists(const QString& file) const;

    void addToListFile(const QStringList& value);
    void openFile(const QString& file, size_t* size, char** buffer);
    void finishLoading();

    bool isOpen(Archive* archive);
    bool isFinishedLoading() const;

    const QStringList& getListFile() const { return listFile; }

private:
    QList<QPair<QString, Archive*>> openedArchives;

    QStringList listFile;
    QMutex listFileMutex;
};

#endif // ARCHIVEMANAGER_H
