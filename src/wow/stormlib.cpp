#include "StormLib.h"

StormLib::StormLib(QObject* parent)
: QLibrary(parent)
{
    resetPointers();
}

StormLib::~StormLib()
{
    close();
}

void StormLib::initialize(const QString& file)
{
    setFileName(file);
}

void StormLib::open()
{
    if(isLoaded())
        return;

    if(load())
    {
        openArchive    = (fpSFileOpenArchive)    resolve("SFileOpenArchive");
        createArchive  = (fpSFileCreateArchive)  resolve("SFileCreateArchive");
        closeArchive   = (fpSFileCloseArchive)   resolve("SFileCloseArchive");
        compactArchive = (fpSFileCompactArchive) resolve("SFileCompactArchive");
        flushArchive   = (fpSFileFlushArchive)   resolve("SFileFlushArchive");
        getFileSize    = (fpSFileGetFileSize)    resolve("SFileGetFileSize");
        addFileEx      = (fpSFileAddFileEx)      resolve("SFileAddFileEx");
        removeFile     = (fpSFileRemoveFile)     resolve("SFileRemoveFile");
        extractFile    = (fpSFileExtractFile)    resolve("SFileExtractFile");
        hasFile        = (fpSFileHasFile)        resolve("SFileHasFile");
        readFile       = (fpSFileReadFile)       resolve("SFileReadFile");
        writeFile      = (fpSFileWriteFile)      resolve("SFileWriteFile");
        finishFile     = (fpSFileFinishFile)     resolve("SFileFinishFile");
        createFile     = (fpSFileCreateFile)     resolve("SFileCreateFile");
        openFileEx     = (fpSFileOpenFileEx)     resolve("SFileOpenFileEx");
        closeFile      = (fpSFileCloseFile)      resolve("SFileCloseFile");
        addListFile    = (fpSFileAddListFile)    resolve("SFileAddListFile");

        // Check if we dont missing anything
        state = openFileEx  && openArchive  && createArchive  && flushArchive
             && addListFile && closeArchive && compactArchive && addFileEx
             && removeFile  && extractFile  && hasFile        && getFileSize
             && readFile    && writeFile    && finishFile     && createFile
             && closeFile   && state ? ReadyToRun : FunctionsNotFound;

        if(state != ReadyToRun)
            unload();
    }
    else
        state = LibraryNotFound;
}

void StormLib::close()
{
    state = Unloaded;

    resetPointers();

    if(isLoaded())
        unload();
}

void StormLib::resetPointers()
{
    openArchive    = 0;
    createArchive  = 0;
    closeArchive   = 0;
    compactArchive = 0;
    flushArchive   = 0;
    addFileEx      = 0;
    removeFile     = 0;
    extractFile    = 0;
    hasFile        = 0;
    getFileSize    = 0;
    readFile       = 0;
    writeFile      = 0;
    finishFile     = 0;
    createFile     = 0;
    openFileEx     = 0;
    closeFile      = 0;
    addListFile    = 0;
}

/* Archive */
bool StormLib::OpenArchive(const char* szMpqName, quint32 dwPriority, quint32 dwFlags, HANDLE* phMPQ)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(openArchive(szMpqName, dwPriority, dwFlags, phMPQ));
}

bool StormLib::CreateArchive(const char* szMpqName, quint32 dwFlags, quint32 dwMaxFileCount, HANDLE* phMPQ)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(createArchive(szMpqName, dwFlags, dwMaxFileCount, phMPQ));
}

bool StormLib::CloseArchive(HANDLE hMpq)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(closeArchive(hMpq));
}

bool StormLib::CompactArchive(HANDLE hMpq, const char* szListFile, bool bReserved)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(compactArchive(hMpq, szListFile, bReserved));
}

bool StormLib::FlushArchive(HANDLE hMpq)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(flushArchive(hMpq));
}

/* Files */
int StormLib::AddListFile(HANDLE hMpq, const char* szFilename)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<int>(addListFile(hMpq, szFilename));
}

bool StormLib::AddFileEx(HANDLE hMpq, const char* szFileName, const char* szArchivedName, quint32 dwFlags, quint32 dwCompression, quint32 dwCompressionNext)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(addFileEx(hMpq, szFileName, szArchivedName, dwFlags, dwCompression, dwCompressionNext));
}

bool StormLib::RemoveFile(HANDLE hMpq, const char* szFileName, quint32 dwSearchScope)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(removeFile(hMpq, szFileName, dwSearchScope));
}

bool StormLib::ExtractFile(HANDLE hMpq, const char* szToExtract, const char* szExtracted, quint32 dwSearchScope)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(extractFile(hMpq, szToExtract, szExtracted, dwSearchScope));
}

bool StormLib::HasFile(HANDLE hMpq, const char* szFileName)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(hasFile(hMpq, szFileName));
}

bool StormLib::CreateFile(HANDLE hMpq, const char* szArchivedName, qulonglong FileTime, quint32 dwFileSize, quint32 lcLocale, quint32 dwFlags, HANDLE* phFile)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(createFile(hMpq, szArchivedName, FileTime, dwFileSize, lcLocale, dwFlags, phFile));
}

bool StormLib::OpenFileEx(HANDLE hMpq, const char* szFilename, quint32 dwSearchScope, HANDLE* phFile)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(openFileEx(hMpq, szFilename, dwSearchScope, phFile));
}

bool StormLib::CloseFile(HANDLE hFile)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(closeFile(hFile));
}

bool StormLib::ReadFile(HANDLE hFile, void* lpBuffer, quint32 dwToRead, quint32* pdwRead)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(readFile(hFile, lpBuffer, dwToRead, pdwRead, NULL));
}

bool StormLib::WriteFile(HANDLE hFile, const void* pvData, quint32 dwSize, quint32 dwCompression)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(writeFile(hFile, pvData, dwSize, dwCompression));
}

bool StormLib::FinishFile(HANDLE hFile)
{
    if(state != ReadyToRun)
        return false;

    return static_cast<bool>(finishFile(hFile));
}

quint32 StormLib::GetFileSize(HANDLE hFile)
{
    if(state == ReadyToRun)
        return static_cast<quint32>(getFileSize(hFile, NULL));

    return 0;
}
