#ifndef STORMLIB_H
#define STORMLIB_H

#include <QLibrary>

// Compression types for multiple compressions
#define MPQ_COMPRESSION_HUFFMANN          0x01  // Huffmann compression (used on WAVE files only)
#define MPQ_COMPRESSION_ZLIB              0x02  // ZLIB compression
#define MPQ_COMPRESSION_PKWARE            0x08  // PKWARE DCL compression
#define MPQ_COMPRESSION_BZIP2             0x10  // BZIP2 compression (added in Warcraft III)
#define MPQ_COMPRESSION_SPARSE            0x20  // Sparse compression (added in Starcraft 2)
#define MPQ_COMPRESSION_ADPCM_MONO        0x40  // IMA ADPCM compression (mono)
#define MPQ_COMPRESSION_ADPCM_STEREO      0x80  // IMA ADPCM compression (stereo)
#define MPQ_COMPRESSION_LZMA              0x12  // LZMA compression. Added in Starcraft 2. This value is NOT a combination of flags.
#define MPQ_COMPRESSION_NEXT_SAME   0xFFFFFFFF  // Same compression

// Flags for SFileCreateArchive
#define MPQ_CREATE_LISTFILE         0x00100000  // Also add the (listfile) file
#define MPQ_CREATE_ATTRIBUTES       0x00200000  // Also add the (attributes) file
#define MPQ_CREATE_SIGNATURE        0x00400000  // Also add the (signature) file
#define MPQ_CREATE_ARCHIVE_V1       0x00000000  // Creates archive of version 1 (size up to 4GB)
#define MPQ_CREATE_ARCHIVE_V2       0x01000000  // Creates archive of version 2 (larger than 4 GB)
#define MPQ_CREATE_ARCHIVE_V3       0x02000000  // Creates archive of version 3
#define MPQ_CREATE_ARCHIVE_V4       0x03000000  // Creates archive of version 4
#define MPQ_CREATE_ARCHIVE_VMASK    0x0F000000  // Mask for archive version

#define FLAGS_TO_FORMAT_SHIFT               24  // (MPQ_CREATE_ARCHIVE_V4 >> FLAGS_TO_FORMAT_SHIFT) => MPQ_FORMAT_VERSION_4

#define STREAM_FLAG_READ_ONLY       0x00000100  // Stream is read only
#define STREAM_FLAG_WRITE_SHARE     0x00000200  // Allow write sharing when open for write
#define STREAM_FLAG_USE_BITMAP      0x00000400  // If the file has a file bitmap, load it and use it
#define STREAM_OPTIONS_MASK         0x0000FF00  // Mask for stream options

#define STREAM_PROVIDERS_MASK       0x000000FF  // Mask to get stream providers
#define STREAM_FLAGS_MASK           0x0000FFFF  // Mask for all stream flags (providers+options)

#define MPQ_OPEN_NO_LISTFILE        0x00010000  // Don't load the internal listfile
#define MPQ_OPEN_NO_ATTRIBUTES      0x00020000  // Don't open the attributes
#define MPQ_OPEN_NO_HEADER_SEARCH   0x00040000  // Don't search for the MPQ header past the begin of the file
#define MPQ_OPEN_FORCE_MPQ_V1       0x00080000  // Always open the archive as MPQ v 1.00, ignore the "wFormatVersion" variable in the header
#define MPQ_OPEN_CHECK_SECTOR_CRC   0x00100000  // On files with MPQ_FILE_SECTOR_CRC, the CRC will be checked when reading file
#define MPQ_OPEN_PATCH              0x00200000  // This archive is a patch MPQ. Used internally.
#define MPQ_OPEN_READ_ONLY          STREAM_FLAG_READ_ONLY

#define MPQ_FILE_IMPLODE            0x00000100  // Implode method (By PKWARE Data Compression Library)
#define MPQ_FILE_COMPRESS           0x00000200  // Compress methods (By multiple methods)
#define MPQ_FILE_ENCRYPTED          0x00010000  // Indicates whether file is encrypted
#define MPQ_FILE_FIX_KEY            0x00020000  // File decryption key has to be fixed
#define MPQ_FILE_PATCH_FILE         0x00100000  // The file is a patch file. Raw file data begin with TPatchInfo structure
#define MPQ_FILE_SINGLE_UNIT        0x01000000  // File is stored as a single unit, rather than split into sectors (Thx, Quantam)
#define MPQ_FILE_DELETE_MARKER      0x02000000  // File is a deletion marker. Used in MPQ patches, indicating that the file no longer exists.
#define MPQ_FILE_SECTOR_CRC         0x04000000  // File has checksums for each sector.
                                                // Ignored if file is not compressed or imploded.
#define MPQ_FILE_SIGNATURE          0x10000000  // Present on STANDARD.SNP\(signature). The only occurence ever observed
#define MPQ_FILE_EXISTS             0x80000000  // Set if file exists, reset when the file was deleted
#define MPQ_FILE_REPLACEEXISTING    0x80000000  // Replace when the file exist (SFileAddFile)

typedef void* HANDLE;

class StormLib : public QLibrary
{
    Q_OBJECT

public:
    enum OpenArchiveFlags
    {
        NO_LISTFILE       = 0x0010, // Don't load the internal listfile
        NO_ATTRIBUTES     = 0x0020, // Don't open the attributes
        MFORCE_MPQ_V1     = 0x0040, // Always open the archive as MPQ v 1.00, ignore the "wFormatVersion" variable in the header
        MCHECK_SECTOR_CRC = 0x0080, // On files with MPQ_FILE_SECTOR_CRC, the CRC will be checked when reading file
        READ_ONLY         = 0x0100, // Open the archive for read-only access
        ENCRYPTED         = 0x0200  // Opens an encrypted MPQ archive (Example: Starcraft II installation)
    };

    enum OpenFileFlags
    {
        FROM_MPQ     = 0x00000000, // Open the file from the MPQ archive
        PATCHED_FILE = 0x00000001, // Open the file from the MPQ archive
        BY_INDEX     = 0x00000002, // The 'szFileName' parameter is actually the file index
        ANY_LOCALE   = 0xFFFFFFFE, // Reserved for StormLib internal use
        LOCAL_FILE   = 0xFFFFFFFF  // Open the file from the MPQ archive
    };

    enum State
    {
        ReadyToRun,
        LibraryNotFound,
        FunctionsNotFound,
        Unloaded
    };

    explicit StormLib(QObject* parent = Q_NULLPTR);
    ~StormLib();

    void initialize(const QString& file);

    void open();
    void close();

    const State getState() const { return state; }

    /// Reimplementation
    bool CreateArchive(const char* szMpqName, quint32 dwFlags, quint32 dwMaxFileCount, HANDLE* phMPQ);
    bool OpenArchive(const char* szMpqName, quint32 dwPriority, quint32 dwFlags, HANDLE* phMPQ);
    bool CloseArchive(HANDLE hMpq);
    bool CompactArchive(HANDLE hMpq, const char * szListFile, bool bReserved);
    bool FlushArchive(HANDLE hMpq);

    int  AddListFile(HANDLE hMpq, const char* szFilename);
    bool AddFileEx(HANDLE hMpq, const char* szFileName, const char* szArchivedName, quint32 dwFlags, quint32 dwCompression, quint32 dwCompressionNext);
    bool CreateFile(HANDLE hMpq, const char* szArchivedName, qulonglong FileTime, quint32 dwFileSize, quint32 lcLocale, quint32 dwFlags, HANDLE* phFile);
    bool RemoveFile(HANDLE hMpq, const char* szFileName, quint32 dwSearchScope);
    bool ExtractFile(HANDLE hMpq, const char* szToExtract, const char* szExtracted, quint32 dwSearchScope);
    bool HasFile(HANDLE hMpq, const char* szFileName);
    bool OpenFileEx(HANDLE hMpq, const char* szFileName, quint32 dwSearchScope, HANDLE* phFile);
    bool CloseFile(HANDLE hFile);
    bool ReadFile(HANDLE hFile, void* lpBuffer, quint32 dwToRead, quint32* pdwRead);
    bool WriteFile(HANDLE hFile, const void* pvData, quint32 dwSize, quint32 dwCompression);
    bool FinishFile(HANDLE hFile);

    quint32 GetFileSize(HANDLE hFile);

private:
    Q_DISABLE_COPY(StormLib)

    void resetPointers();

    State state = Unloaded;

    /// Reimplementation
    typedef bool (__stdcall* fpSFileOpenArchive)(const char*, quint32, quint32, HANDLE*);
    typedef bool (__stdcall* fpSFileCreateArchive)(const char*, quint32, quint32, HANDLE*);
    typedef bool (__stdcall* fpSFileCloseArchive)(HANDLE);
    typedef bool (__stdcall* fpSFileCompactArchive)(HANDLE, const char*, bool);
    typedef bool (__stdcall* fpSFileFlushArchive)(HANDLE);

    typedef int  (__stdcall* fpSFileAddListFile)(HANDLE, const char*);
    typedef bool (__stdcall* fpSFileReadFile)(HANDLE, void*, quint32, quint32*, void*);
    typedef bool (__stdcall* fpSFileWriteFile)(HANDLE, const void*, quint32, quint32);
    typedef bool (__stdcall* fpSFileFinishFile)(HANDLE);
    typedef bool (__stdcall* fpSFileAddFileEx)(HANDLE, const char*, const char*, quint32, quint32, quint32);
    typedef bool (__stdcall* fpSFileRemoveFile)(HANDLE, const char*, quint32);
    typedef bool (__stdcall* fpSFileExtractFile)(HANDLE, const char*, const char*, quint32);
    typedef bool (__stdcall* fpSFileHasFile)(HANDLE, const char*);
    typedef bool (__stdcall* fpSFileCreateFile)(HANDLE, const char*, qulonglong, quint32, quint32, quint32, HANDLE*);
    typedef bool (__stdcall* fpSFileOpenFileEx)(HANDLE, const char*, quint32, HANDLE*);
    typedef bool (__stdcall* fpSFileCloseFile)(HANDLE);

    typedef quint32 (__stdcall* fpSFileGetFileSize)(HANDLE, quint32);

    fpSFileOpenArchive    openArchive;
    fpSFileCreateArchive  createArchive;
    fpSFileCloseArchive   closeArchive;
    fpSFileCompactArchive compactArchive;
    fpSFileFlushArchive   flushArchive;

    fpSFileAddListFile addListFile;
    fpSFileAddFileEx   addFileEx;
    fpSFileRemoveFile  removeFile;
    fpSFileExtractFile extractFile;
    fpSFileHasFile     hasFile;
    fpSFileReadFile    readFile;
    fpSFileWriteFile   writeFile;
    fpSFileFinishFile  finishFile;
    fpSFileCreateFile  createFile;
    fpSFileOpenFileEx  openFileEx;
    fpSFileCloseFile   closeFile;

    fpSFileGetFileSize getFileSize;
};

#endif // STORMLIB_H
