#ifndef WDL_H
#define WDL_H

#include <QObject>
#include <QVector>

#include "Chunk.h"

class WDL
{
public:
    WDL();
    explicit WDL(const QString& file);

    bool save();

private:
    QString fileName = "none";

    Chunk MVER;
    Chunk MWMO;
    Chunk MWID;
    Chunk MODF;
    Chunk MAOF;

    QVector<Chunk> MARE_MAHO;
};

#endif // WDL_H
