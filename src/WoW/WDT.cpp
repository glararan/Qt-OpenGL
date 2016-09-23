#include "WDT.h"

#include <QFile>
#include <QDebug>

#include "ChunkHeaders.h"

namespace WoW
{
    WDT::WDT()
    {
        /*QVector<QChar> dataMVER;
        QVector<QChar> dataMPHD(32);
        QVector<QChar> dataMAIN(32768);

        dataMVER.append(18 & 0xff);
        dataMVER.append((18 >> 8)  & 0xff);
        dataMVER.append((18 >> 16) & 0xff);
        dataMVER.append((18 >> 24) & 0xff);*/

        // Chunks
        MverHeader mVer;
        mVer.version = 18;

        MVER = Chunk<MverHeader>("REVM", mVer);
        MPHD = Chunk<MphdHeader>("DHPM");
        MAIN = Chunk<MainHeader>("NIAM");
    }

    WDT::WDT(const QString& file)
    : fileName(file)
    {
        QFile wdt(file);

        if(wdt.open(QIODevice::ReadOnly))
        {
            int offset = 0;

            MVER = Chunk<MverHeader>(wdt, offset);
            MPHD = Chunk<MphdHeader>(wdt, offset);
            MAIN = Chunk<MainHeader>(wdt, offset);

            const int flags = MPHD.getOffset(0);

            if(flags & 1)
                MWMO = Chunk<MwmoHeader>(wdt, offset);

            if(MWMO.getSize() > 0)
                MODF = Chunk<ModfHeader>(wdt, offset);

            wdt.close();
        }
        else
            qDebug() << QObject::tr("Error while opening WDT file %1").arg(file);
    }

    bool WDT::getADT(const int& x, const int& y)
    {
        return MAIN.getStructure().ADTs[x][y].exist > 0 ? true : false;
    }

    void WDT::setADT(const quint32& value, const int& x, const int& y)
    {
        MainHeader header = MAIN.getStructure();
        header.ADTs[x][y].exist = value;

        MAIN.setStructure(header);
    }

    void WDT::setFlags(const quint32& value)
    {
        MphdHeader header = MPHD.getStructure();
        header.flags      = value;

        MPHD.setStructure(header);
    }

    bool WDT::save(const QString& altFileName)
    {
        QFile file(fileName);

        if(!altFileName.isEmpty())
            file.setFileName(altFileName);

        if(file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        {
            file.write(MVER.getChunk());
            file.write(MPHD.getChunk());
            file.write(MAIN.getChunk());

            if(!MWMO.isEmpty())
                file.write(MWMO.getChunk());

            if(!MODF.isEmpty())
                file.write(MODF.getChunk());

            file.close();

            return true;
        }

        return false;
    }
}
