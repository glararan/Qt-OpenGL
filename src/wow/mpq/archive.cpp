#include "Archive.h"

#include <Core/Singleton.h>

#include "../StormLib.h"
#include "ArchiveManager.h"

#include <QDebug>

#define stormLib   Singleton<StormLib>::instancePtr()
#define ArchiveMgr Singleton<ArchiveManager>::instancePtr()

// todo use qPrintable instead of toStdString.c_str()?

Archive::Archive(const QString& file, bool processListFile, bool create, QObject* parent)
: QObject(parent)
{
    if(create)
    {
        if(!stormLib->CreateArchive(file.toStdString().c_str(), MPQ_CREATE_ARCHIVE_V2 | MPQ_CREATE_ATTRIBUTES, 0x40, &handler))
            qDebug() << tr("Error while creating archive: ") << file;
        else
            qDebug() << tr("MPQ Archive successfully created and opened: ") << file;
    }
    else
    {
        if(!stormLib->OpenArchive(file.toStdString().c_str(), 0, MPQ_OPEN_NO_LISTFILE | MPQ_OPEN_READ_ONLY, &handler))
            qDebug() << tr("Error while opening archive: ") << file;
        else
            qDebug() << tr("MPQ Archive successfully opened: ") << file;
    }

    finished = !processListFile;

    if(finished)
        emit loadingFinished();
}

Archive::~Archive()
{
    if(handler)
        stormLib->CloseArchive(handler);
}

void Archive::addFile(ArchiveFile* file, QString mpqPath)
{
    HANDLE fileHandler;

    if(stormLib->CreateFile(handler, mpqPath.toStdString().c_str(), 0, file->getSize(), 0, MPQ_FILE_COMPRESS | MPQ_FILE_ENCRYPTED | MPQ_FILE_REPLACEEXISTING, &fileHandler))
        qDebug() << tr("MPQ Archive %1 successfully created file %2");
    else
    {
        qDebug() << tr("Error: %1, while creating file %2");

        return;
    }

    if(stormLib->WriteFile(fileHandler, file->getBuffer(), sizeof(file->getBuffer()), MPQ_COMPRESSION_ZLIB))
        qDebug() << tr("Successfully written data into file %1 inside MPQ Archive %2");
    else
    {
        qDebug() << tr("Error: %1, while writing data to file %2");

        return;
    }

    if(stormLib->FinishFile(fileHandler))
        qDebug() << tr("Successfully finished file %1 inside MPQ Archive %2");
    else
        qDebug() << tr("Error: %1, while finishing file %2");
}

bool Archive::hasFile(const QString& fileName) const
{
    return stormLib->HasFile(handler, fileName.toStdString().c_str());
}

bool Archive::readFile(const QString& fileName, size_t* size, char** buffer) const
{
    HANDLE fileHandler;

    bool opened = stormLib->OpenFileEx(handler, fileName.toStdString().c_str(), 0, &fileHandler);

    if(opened)
    {
        *size = stormLib->GetFileSize(fileHandler);
        *buffer = new char[*size];

        quint32 read;

        stormLib->ReadFile(fileHandler, *buffer, *size, &read);

        if(read != *size)
            qDebug() << tr("Error while reading %1! Size is not equal read buffer");

        stormLib->CloseFile(fileHandler);
    }

    return opened;
}

void Archive::finishLoading()
{
    if(finished)
        return;

    finished = true;

    HANDLE fileHandler;

    if(stormLib->OpenFileEx(handler, "(listfile)", 0, &fileHandler))
    {
        const quint32 size = stormLib->GetFileSize(fileHandler);

        char* buffer = new char[size];

        quint32 read;

        stormLib->ReadFile(fileHandler, buffer, size, &read);
        stormLib->CloseFile(fileHandler);

        if(read != size)
            qDebug() << tr("Error while reading listfile! Size is not equal read buffer");

        listFile = QString::fromLatin1(buffer, size).toLower().split("\r\n", QString::SkipEmptyParts);

        ArchiveMgr->addToListFile(listFile);

        delete[] buffer;
    }

    emit loadingFinished();
}

void Archive::save()
{
    if(stormLib->CompactArchive(handler, Q_NULLPTR, false) && stormLib->CloseArchive(handler))
        qDebug() << tr("MPQ Archive %1 was sucessfully saved to disk");
    else
        qDebug() << tr("Error while saving MPQ Archive %1 to disk");
}
