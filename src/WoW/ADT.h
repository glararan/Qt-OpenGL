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

        Chunk MVER;
        Chunk MHDR;
        Chunk MCIN;
        Chunk MTEX;
        Chunk MMDX;
        Chunk MMID;
        Chunk MWMO;
        Chunk MWID;
        Chunk MDDF;
        Chunk MODF;
        Chunk MH2O;
        Chunk MCNK[CELLS * CELLS];
        //Chunk MCNK[CELLS * CELLS];
        Chunk MFBO;
        Chunk MTFX;
    };
}

#endif // ADT_H
