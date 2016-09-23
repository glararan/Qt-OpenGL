#include "WDL.h"

#include "ChunkHeaders.h"

#include <QFile>
#include <QDebug>

namespace WoW
{
    WDL::WDL()
    {
        // Chunks
        MVER = Chunk<MverHeader>("REVM", sizeof(MverHeader), QByteArray::number(18));
        MWMO = Chunk<MwmoHeader>("OMWM");
        MWID = Chunk<MwidHeader>("DIWM");
        MODF = Chunk<ModfHeader>("FDOM");
        MAOF = Chunk<MaofHeader>("OFAM");
    }

    WDL::WDL(const QString& file)
    {
        QFile wdl(file);

        if(wdl.open(QIODevice::ReadOnly))
        {
            int offset = 0;

            MVER = Chunk<MverHeader>(wdl, offset);
            MWMO = Chunk<MwmoHeader>(wdl, offset);
            MWID = Chunk<MwidHeader>(wdl, offset);
            MODF = Chunk<ModfHeader>(wdl, offset);
            MAOF = Chunk<MaofHeader>(wdl, offset);

            while(offset < wdl.size())
                MARE_MAHO.append(BaseChunk(wdl, offset));

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

            for(const BaseChunk& chunk : MARE_MAHO)
                file.write(chunk.getChunk());

            file.close();

            return true;
        }

        return false;
    }
}
