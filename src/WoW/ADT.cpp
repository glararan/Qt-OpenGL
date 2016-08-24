#include "ADT.h"

#include <QVector>
#include <QDebug>

namespace WoW
{
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

        for(int i = 0; i < CELLS * CELLS; ++i)
            MCNK[i] = Chunk("KNCM", sizeof(McnkHeader), QByteArray());

        MFBO = Chunk("OBFM", sizeof(MfboHeader), QByteArray());
        MTFX = Chunk("XFTM", sizeof(MtfxHeader), QByteArray());
    }

    ADT::ADT(const QString& file)
    : fileName(file)
    {
        QFile adt(file);

        if(adt.open(QIODevice::ReadOnly))
        {
            int offset = 0;

            MVER = Chunk(adt, offset);
            MHDR = Chunk(adt, offset);

            MhdrHeader mhdr = MHDR.getStructure<MhdrHeader>();

            MCIN = Chunk(adt, offset = 0x14 + mhdr.mcinOffset);
            {
                MTEX = Chunk(adt, offset = 0x14 + mhdr.mtexOffset);

                offset = 0;

                QVector<QString> textures;

                while(offset < MTEX.getSize())
                {
                    QString texture((char*)(MTEX.getData().data() + offset));

                    textures.append(texture);

                    offset += texture.size() + 1;
                }

                if(textures.isEmpty())
                    textures.append("tileset\\generic\\black.blp");
            }
            MMDX = Chunk(adt, offset = 0x14 + mhdr.mmdxOffset);
            MMID = Chunk(adt, offset = 0x14 + mhdr.mmidOffset);
            MWMO = Chunk(adt, offset = 0x14 + mhdr.mwmoOffset);
            MWID = Chunk(adt, offset = 0x14 + mhdr.mwidOffset);
            MDDF = Chunk(adt, offset = 0x14 + mhdr.mddfOffset);
            MODF = Chunk(adt, offset = 0x14 + mhdr.modfOffset);

            if(mhdr.mh2oOffset)
                MH2O = Chunk(adt, offset = 0x14 + mhdr.mh2oOffset);

            McinHeader mcinHeader = MCIN.getStructure<McinHeader>();

            for(int i = 0; i < CELLS * CELLS; ++i)
            {
                //MCNK[i] = QSharedPointer<McnkHeader>(&MCIN.getStructure<McinHeader>().entries[i]);

                MCNK[i] = Chunk(adt, offset = mcinHeader.entries[i].offset);

                // todo own class with subchunks like mcvt, mclv,...
            }

            if((mhdr.flags & 1) && mhdr.mfboOffset)
                MFBO = Chunk(adt, offset = 0x14 + mhdr.mfboOffset);

            if(mhdr.mtfxOffset)
                MTFX = Chunk(adt, offset = 0x14 + mhdr.mtfxOffset);

            adt.close();
        }
        else
            qDebug() << QObject::tr("Error while opening ADT file %1").arg(file);
    }

    bool ADT::save(const QString& altFileName)
    {
        QFile file(fileName);

        if(!altFileName.isEmpty())
            file.setFileName(altFileName);

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

            for(int i = 0; i < CELLS * CELLS; ++i)
                file.write(MCNK[i].getChunk());

            file.write(MFBO.getChunk());
            file.write(MTFX.getChunk());

            file.close();

            return true;
        }

        return false;
    }

    void ADT::update()
    {

    }
}
