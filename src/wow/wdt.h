#ifndef WDT_H
#define WDT_H

#include <QObject>

#include "Chunk.h"

class WDT
{
public:
    WDT();
    explicit WDT(const QString& file);

    bool getADT(const int& x, const int& y);

    void setADT(const quint32& value, const int& x, const int& y);

    bool save();

private:
    QString fileName = "none";

    Chunk MVER;
    Chunk MPHD;
    Chunk MAIN;
    Chunk MWMO;
    Chunk MODF;
};

#endif // WDT_H
