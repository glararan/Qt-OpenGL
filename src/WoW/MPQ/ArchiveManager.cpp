#include "ArchiveManager.h"

#include "../StormLib.h"
#include "../Manager.h"

#include <Core/Singleton.h>

#include <QStringList>

#define stormLib Singleton<StormLib>::instancePtr()
#define WoWMgr   Singleton<Manager>::instancePtr()

namespace WoW
{
    ArchiveManager::ArchiveManager(QObject* parent)
    : QObject(parent)
    {
        stormLib->open();
    }

    ArchiveManager::~ArchiveManager()
    {
        loadingThread.quit();
        loadingThread.wait();

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

    void ArchiveManager::openCommonArchives(const Version& version)
    {
        QStringList archives;

        switch(version)
        {
            case Classic:
                break;

            case TBC:
                break;

            case WotLK: // archives coppied from Noggit
                {
                    archives << "common.MPQ"
                             << "common-2.MPQ"
                             << "expansion.MPQ"
                             << "lichking.MPQ"
                             << "patch.MPQ"
                             << "patch-%1.MPQ"
                             << "{locale}/locale-{locale}.MPQ"
                             << "{locale}/expansion-locale-{locale}.MPQ"
                             << "{locale}/lichking-locale-{locale}.MPQ"
                             << "{locale}/patch-{locale}.MPQ"
                             << "{locale}/patch-{locale}-%1.MPQ";
                    
                    for(const QString& archive : archives)
                    {
                        QString filePath(WoWMgr->getGameDir().absoluteFilePath("Data/" + archive));

                        if(filePath.contains("{locale}"))
                            filePath.replace("{locale}", WoWMgr->getGameLocale());

                        if(filePath.contains("%1"))
                        {
                            for(char i('2'); i <= '9'; ++i)
                            {
                                const QString newFilePath(filePath.arg(i));

                                if(QFile::exists(newFilePath))
                                    load(newFilePath, true);
                            }

                            continue;
                        }

                        if(QFile::exists(filePath))
                            load(filePath, true);
                    }
                }
                break;

            case Cataclysm:
                break;

            case MoP:
                break;

            case Legion:
                break;
        }
    }

    Archive* ArchiveManager::create(const QString& file, const bool processListFile)
    {
        Archive* archive(new Archive(file, processListFile, true, this));
        archive->moveToThread(&loadingThread);

        //connect(this, &ArchiveManager::operate, worker, &AsyncWorker::process);
        //connect(worker, &AsyncWorker::resultReady, this, &ArchiveManager::handleResults);

        openedArchives.append(QPair<QString, Archive*>(file, archive));

        if(!loadingThread.isRunning())
            loadingThread.start();

        return archive;
    }

    Archive* ArchiveManager::load(const QString& file, const bool processListFile)
    {
        Archive* archive(new Archive(file, processListFile, false, this));
        archive->moveToThread(&loadingThread);

        //connect(this, &ArchiveManager::operate, worker, &AsyncWorker::process);
        //connect(worker, &AsyncWorker::resultReady, this, &ArchiveManager::handleResults);

        openedArchives.append(QPair<QString, Archive*>(file, archive));

        if(!loadingThread.isRunning())
            loadingThread.start();

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
            if(!pair.second->isFinished())
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
            pair.second->process();
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
}
