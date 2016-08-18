#include "ADT.h"

#include "ChunkHeaders.h"

ADT::ADT()
{
    MVER = Chunk("REVM", sizeof(MverHeader), QByteArray::number(18));
    MHDR = Chunk("RDHM", sizeof(MhdrHeader), QByteArray());
    MCIN = Chunk("NICM", sizeof(McinHeader), QByteArray());
    MTEX = Chunk("XETM", sizeof(MtexHeader), QByteArray());
    MMDX = Chunk("XDMM", sizeof(MmdxHeader), QByteArray());
    MMID = Chunk("DIMM", sizeof(MmidHeader), QByteArray());
    MWMO = Chunk("OMWM", sizeof(MwmoHeader), QByteArray());
    MWID = Chunk("DIWM", sizeof(MwidHeader), QByteArray());
    MDDF = Chunk("FDDM", sizeof(MddfHeader), QByteArray());
    MODF = Chunk("FDOM", sizeof(ModfHeader), QByteArray());
    MH2O = Chunk("O2HM", sizeof(Mh2oHeader), QByteArray());

    for(int i = 0; i < CELLS_PER_GRID * CELLS_PER_GRID; ++i)
        MCNK[i] = Chunk("KNCM", sizeof(McnkHeader), QByteArray());

    MFBO = Chunk("OBFM", sizeof(MfboHeader), QByteArray());
    MTXF = Chunk("FXTM", sizeof(MtxfHeader), QByteArray());
}

ADT::ADT(const QString& file)
: fileName(file)
{

}

bool ADT::save()
{
    QFile file(fileName);

    if(file.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        file.write(MVER.getChunk());
        file.write(MHDR.getChunk());
        file.write(MCIN.getChunk());
        file.write(MTEX.getChunk());
        file.write(MMDX.getChunk());
        file.write(MMID.getChunk());
        file.write(MWMO.getChunk());
        file.write(MWID.getChunk());
        file.write(MDDF.getChunk());
        file.write(MODF.getChunk());
        file.write(MH2O.getChunk());

        for(int i = 0; i < CELLS_PER_GRID * CELLS_PER_GRID; ++i)
            file.write(MCNK[i].getChunk());

        file.write(MFBO.getChunk());
        file.write(MTXF.getChunk());

        file.close();

        return true;
    }

    return false;
}

void ADT::update()
{

}
