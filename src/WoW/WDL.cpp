#include "WDL.h"

#include "ChunkHeaders.h"

#include <QFile>
#include <QDebug>

namespace WoW
{
    WDL::WDL()
    {
        // Chunks
        MVER = Chunk("REVM", sizeof(MverHeader), QByteArray::number(18));
        MWMO = Chunk("OMWM", sizeof(MwmoHeader), QByteArray());
        MWID = Chunk("DIWM", sizeof(MwidHeader), QByteArray());
        MODF = Chunk("FDOM", sizeof(ModfHeader), QByteArray());
        MAOF = Chunk("OFAM", sizeof(MaofHeader), QByteArray());
    }

    WDL::WDL(const QString& file)
    {
        QFile wdl(file);

        if(wdl.open(QIODevice::ReadOnly))
        {
            int offset = 0;

            MVER = Chunk(wdl, offset);
            MWMO = Chunk(wdl, offset);
            MWID = Chunk(wdl, offset);
            MODF = Chunk(wdl, offset);
            MAOF = Chunk(wdl, offset);

            while(offset < wdl.size())
                MARE_MAHO.append(Chunk(wdl, offset));

            wdl.close();
        }
        else
            qDebug() << QObject::tr("Error while opening WDL file %1").arg(file);
    }

    bool WDL::save(const QString& altFileName)
    {
        QFile file(fileName);

        if(!altFileName.isEmpty())
            file.setFileName(altFileName);

        if(file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        {
            file.write(MVER.getChunk());
            file.write(MWMO.getChunk());
            file.write(MWID.getChunk());
            file.write(MODF.getChunk());
            file.write(MAOF.getChunk());

            for(const Chunk& chunk : MARE_MAHO)
                file.write(chunk.getChunk());

            file.close();

            return true;
        }

        return false;
    }
}
