#ifndef ARCHIVEMANAGER_H
#define ARCHIVEMANAGER_H

#include <QObject>
#include <QMutex>

#include "../Common.h"
#include "Archive.h"

namespace WoW
{
    class ArchiveManager : public QObject
    {
        Q_OBJECT

    public:
        explicit ArchiveManager(QObject* parent = Q_NULLPTR);
        ~ArchiveManager();

        void openCommonArchives(const Version& version);

        Archive* create(const QString& file, const bool processListFile = false);
        Archive* load(const QString& file, const bool processListFile = false);

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
}

#endif // ARCHIVEMANAGER_H
