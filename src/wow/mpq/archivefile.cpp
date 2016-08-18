#include "ArchiveFile.h"

#include "ArchiveManager.h"

#include <Core/Singleton.h>

#include <QDebug>

#define ArchiveMgr Singleton<ArchiveManager>::instancePtr()

ArchiveFile::ArchiveFile(const QString& file, const bool& create, QObject* parent)
: QObject(parent)
, fileName(file)
{
    if(!exists(file))
    {
        if(!create)
        {
            // throw
        }

        onDisk = create;
    }
    else
    {
        //
    }
}

ArchiveFile::~ArchiveFile()
{
    close();
}

void ArchiveFile::close()
{
    delete[] buffer;

    buffer = Q_NULLPTR;

    endOfFile = true;
}

bool ArchiveFile::exists(const QString& file)
{
    return ArchiveMgr->exists(file);// || diskSearchPath();
}

void ArchiveFile::setBuffer(char* value, size_t length)
{
    delete buffer;

    buffer = value;
    size   = length;
}

void ArchiveFile::save()
{

}

void ArchiveFile::saveToMPQ(Archive* archive, const QString& mpqPath)
{
    if(!ArchiveMgr->isOpen(archive))
    {
        qDebug() << tr("Archive is not opened to save file into MPQ");

        return;
    }

    QString path = mpqPath.isEmpty() ? QString("/") : QString(mpqPath).replace(QString("/"), QString("\\"));

    archive->addFile(this, path);
}
