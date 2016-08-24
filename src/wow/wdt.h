#ifndef WDT_H
#define WDT_H

#include <QObject>

#include "Common.h"
#include "Chunk.h"

namespace WoW
{
    class WDT
    {
    public:
        WDT();
        explicit WDT(const QString& file);

        bool getADT(const int& x, const int& y);

        void setADT(const quint32& value, const int& x, const int& y);

        bool save(const QString& altFileName = QString());

    private:
        QString fileName = "none";

        Version version = WotLK;

        Chunk MVER;
        Chunk MPHD;
        Chunk MAIN;
        Chunk MWMO;
        Chunk MODF;
    };
}

#endif // WDT_H
