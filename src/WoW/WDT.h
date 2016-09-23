#ifndef WDT_H
#define WDT_H

#include <QObject>

#include "Common.h"
#include "Chunk.h"
#include "ChunkHeaders.h"

namespace WoW
{
    class WDT
    {
    public:
        enum Data
        {
            MapNotExists,
            MapExists
        };

        WDT();
        explicit WDT(const QString& file);

        bool getADT(const int& x, const int& y);

        void setADT(const quint32& value, const int& x, const int& y);
        void setFlags(const quint32& value);

        bool save(const QString& altFileName = QString());

    private:
        QString fileName = "none";

        Version version = WotLK;

        Chunk<MverHeader> MVER;
        Chunk<MphdHeader> MPHD;
        Chunk<MainHeader> MAIN;
        Chunk<MwmoHeader> MWMO;
        Chunk<ModfHeader> MODF;
    };
}

#endif // WDT_H
