#ifndef ADT_H
#define ADT_H

#include <QObject>

#include "Chunk.h"

#define TILESIZE  533.33333f
#define CHUNKSIZE TILESIZE  / 16.0f
#define UNITSIZE  CHUNKSIZE / 8.0f

#define CELLS_PER_GRID 16
#define CELL_SIZE      8
#define GRID_SIZE      CELLS_PER_GRID * CELL_SIZE

class ADT
{
public:
    ADT();
    explicit ADT(const QString& file);

    bool save();

private:
    void update(); // updates or create MHDR & MCIN

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
    Chunk MCNK[CELLS_PER_GRID * CELLS_PER_GRID];
    Chunk MFBO;
    Chunk MTXF;
};

#endif // ADT_H
