#ifndef WDL_H
#define WDL_H

#include <QObject>
#include <QVector>

#include "Common.h"
#include "Chunk.h"

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

        Chunk MVER;
        Chunk MWMO;
        Chunk MWID;
        Chunk MODF;
        Chunk MAOF;

        QVector<Chunk> MARE_MAHO;
    };
}

#endif // WDL_H
