#ifndef ADT_H
#define ADT_H

#include <QObject>
#include <QSharedPointer>

#include "Common.h"
#include "Chunk.h"
#include "ChunkHeaders.h"

#define TILESIZE  533.33333f
#define CHUNKSIZE TILESIZE  / 16.0f
#define UNITSIZE  CHUNKSIZE / 8.0f

#define CELLS     16
#define CELL_SIZE 8
#define GRID_SIZE CELLS * CELL_SIZE

namespace WoW
{
    class ADT
    {
    public:
        ADT();
        explicit ADT(const QString& file);

        bool save(const QString& altFileName = QString());

    private:
        void update(); // updates or create MHDR & MCIN

        Version version = WotLK;

        QString fileName;

        Chunk<MverHeader> MVER;
        Chunk<MhdrHeader> MHDR;
        Chunk<McinHeader> MCIN;
        Chunk<MtexHeader> MTEX;
        Chunk<MmdxHeader> MMDX;
        Chunk<MmidHeader> MMID;
        Chunk<MwmoHeader> MWMO;
        Chunk<MwidHeader> MWID;
        Chunk<MddfHeader> MDDF;
        Chunk<ModfHeader> MODF;
        Chunk<Mh2oHeader> MH2O;
        Chunk<McnkHeader> MCNK[CELLS * CELLS];
        Chunk<MfboHeader> MFBO;
        Chunk<MtfxHeader> MTFX;
    };
}

#endif // ADT_H
