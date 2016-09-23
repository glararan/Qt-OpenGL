#ifndef WDL_H
#define WDL_H

#include <QObject>
#include <QVector>

#include "Common.h"
#include "Chunk.h"
#include "ChunkHeaders.h"

namespace WoW
{
    class WDL
    {
    public:
        WDL();
        explicit WDL(const QString& file);

        bool save(const QString& altFileName = QString());

    private:
        QString fileName = "none";

        Version version = WotLK;

        Chunk<MverHeader> MVER;
        Chunk<MwmoHeader> MWMO;
        Chunk<MwidHeader> MWID;
        Chunk<ModfHeader> MODF;
        Chunk<MaofHeader> MAOF;

        QVector<BaseChunk> MARE_MAHO;
    };
}

#endif // WDL_H
