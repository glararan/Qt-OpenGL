#include "ArchiveManager.h"

#include "../StormLib.h"

#include <Core/Singleton.h>

#include <QStringList>

#define stormLib Singleton<StormLib>::instancePtr()

ArchiveManager::ArchiveManager(QObject* parent)
: QObject(parent)
{
    stormLib->open();
}

ArchiveManager::~ArchiveManager()
{
    unloadAll();

    stormLib->close();
}

void ArchiveManager::addToListFile(const QStringList& value)
{
    QMutexLocker locker(&listFileMutex);

    listFile.append(value);

    if(isFinishedLoading())
    {
        listFile.sort();
        listFile.removeDuplicates();
    }
}

Archive* ArchiveManager::create(const QString& file, bool processListFile)
{
    Archive* archive = new Archive(file, processListFile, true, this);

    openedArchives.push_back(QPair<QString, Archive*>(file, archive));
    // async loader add

    return archive;
}

Archive* ArchiveManager::load(const QString& file, bool processListFile)
{
    Archive* archive = new Archive(file, processListFile, false, this);

    openedArchives.push_back(QPair<QString, Archive*>(file, archive));
    // async loader add

    return archive;
}

void ArchiveManager::unload(const QString& file)
{
    for(const QPair<QString, Archive*> pair : openedArchives)
    {
        if(pair.first == file)
        {
            delete pair.second;

            openedArchives.removeOne(pair);
        }
    }
}

void ArchiveManager::unloadAll()
{
    for(const QPair<QString, Archive*> pair : openedArchives)
        delete pair.second;

    openedArchives.clear();
}

bool ArchiveManager::isFinishedLoading() const
{
    for(const QPair<QString, Archive*> pair : openedArchives)
    {
        if(!pair.second->isLoadingFinished())
            return false;
    }

    return true;
}

bool ArchiveManager::isOpen(Archive* archive)
{
    for(const QPair<QString, Archive*> pair : openedArchives)
    {
        if(pair.second == archive)
            return true;
    }

    return false;
}

void ArchiveManager::finishLoading()
{
    for(const QPair<QString, Archive*> pair : openedArchives)
        pair.second->finishLoading();
}

bool ArchiveManager::exists(const QString& file) const
{
    for(const QPair<QString, Archive*> pair : openedArchives)
    {
        if(pair.second->hasFile(file))
            return true;
    }

    return false;
}

void ArchiveManager::openFile(const QString& file, size_t* size, char** buffer)
{

}
