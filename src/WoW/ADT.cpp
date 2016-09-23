#include "ADT.h"

#include <QBuffer>
#include <QVector>
#include <QDebug>

namespace WoW
{
    ADT::ADT()
    {
        MVER = Chunk<MverHeader>("REVM", QByteArray::number(18));
        MHDR = Chunk<MhdrHeader>("RDHM");
        MCIN = Chunk<McinHeader>("NICM");
        MTEX = Chunk<MtexHeader>("XETM");
        MMDX = Chunk<MmdxHeader>("XDMM");
        MMID = Chunk<MmidHeader>("DIMM");
        MWMO = Chunk<MwmoHeader>("OMWM");
        MWID = Chunk<MwidHeader>("DIWM");
        MDDF = Chunk<MddfHeader>("FDDM");
        MODF = Chunk<ModfHeader>("FDOM");
        MH2O = Chunk<Mh2oHeader>("O2HM");

        for(int i = 0; i < CELLS * CELLS; ++i)
            MCNK[i] = Chunk<McnkHeader>("KNCM");

        MFBO = Chunk<MfboHeader>("OBFM");
        MTFX = Chunk<MtfxHeader>("XFTM");
    }

    ADT::ADT(const QString& file)
    : fileName(file)
    {
        QFile adt(file);

        if(adt.open(QIODevice::ReadOnly))
        {
            int offset = 0;

            MVER = Chunk<MverHeader>(adt, offset);
            MHDR = Chunk<MhdrHeader>(adt, offset);

            MhdrHeader mhdr = MHDR.getStructure();

            MCIN = Chunk<McinHeader>(adt, offset = 0x14 + mhdr.mcinOffset);
            {
                MTEX = Chunk<MtexHeader>(adt, offset = 0x14 + mhdr.mtexOffset);

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
            MMDX = Chunk<MmdxHeader>(adt, offset = 0x14 + mhdr.mmdxOffset);
            MMID = Chunk<MmidHeader>(adt, offset = 0x14 + mhdr.mmidOffset);
            MWMO = Chunk<MwmoHeader>(adt, offset = 0x14 + mhdr.mwmoOffset);
            MWID = Chunk<MwidHeader>(adt, offset = 0x14 + mhdr.mwidOffset);
            MDDF = Chunk<MddfHeader>(adt, offset = 0x14 + mhdr.mddfOffset);
            MODF = Chunk<ModfHeader>(adt, offset = 0x14 + mhdr.modfOffset);

            if(mhdr.mh2oOffset)
                MH2O = Chunk<Mh2oHeader>(adt, offset = 0x14 + mhdr.mh2oOffset);

            McinHeader mcinHeader = MCIN.getStructure();

            for(int i = 0; i < CELLS * CELLS; ++i)
            {
                //MCNK[i] = QSharedPointer<McnkHeader>(&MCIN.getStructure<McinHeader>().entries[i]);

                MCNK[i] = Chunk<McnkHeader>(adt, offset = mcinHeader.entries[i].offset);

                // todo own class with subchunks like mcvt, mclv,...
            }

            if((mhdr.flags & 1) && mhdr.mfboOffset)
                MFBO = Chunk<MfboHeader>(adt, offset = 0x14 + mhdr.mfboOffset);

            if(mhdr.mtfxOffset)
                MTFX = Chunk<MtfxHeader>(adt, offset = 0x14 + mhdr.mtfxOffset);

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
            QBuffer buffer;
            buffer.open(QBuffer::ReadWrite);
            buffer.write(MVER.getChunk());
            buffer.write(MHDR.getChunk());

            // todo!: get data pointer from buffer.pos() - sizeof(MhdrHeader) => header
            MhdrHeader* header = (MhdrHeader*)buffer.data().data()[buffer.pos() - sizeof(MhdrHeader)];

            //MhdrHeader header = MHDR.getStructure<MhdrHeader>();
            header->mcinOffset = buffer.pos() - 0x14;
            buffer.write(MCIN.getChunk());

            McinHeader* chunkHeader = (McinHeader*)buffer.data().data()[buffer.pos() - sizeof(McinHeader)];
            //McinHeader chunkHeader = MCIN.getStructure<McinHeader>();

            header->mtexOffset = buffer.pos() - 0x14;
            buffer.write(MTEX.getChunk());

            header->mmdxOffset = buffer.pos() - 0x14;
            buffer.write(MMDX.getChunk());

            header->mmidOffset = buffer.pos() - 0x14;
            buffer.write(MMID.getChunk());

            header->mwmoOffset = buffer.pos() - 0x14;
            buffer.write(MWMO.getChunk());

            header->mwidOffset = buffer.pos() - 0x14;
            buffer.write(MWID.getChunk());

            header->mddfOffset = buffer.pos() - 0x14;
            buffer.write(MDDF.getChunk());

            header->modfOffset = buffer.pos() - 0x14;
            buffer.write(MODF.getChunk());

            if(MH2O.getSize())
            {
                header->mh2oOffset = buffer.pos() - 0x14;
                buffer.write(MH2O.getChunk());
            }
            else
                header->mh2oOffset = 0;

            for(int i = 0; i < CELLS * CELLS; ++i)
            {
                chunkHeader->entries[i].offset  = buffer.pos();
                chunkHeader->entries[i].size    = MCNK[i].getSize();
                chunkHeader->entries[i].asyncID = 0x20; // 32
                chunkHeader->entries[i].flags   = 0x20; // 32

                buffer.write(MCNK[i].getChunk());
            }

            if(MFBO.getSize())
            {
                header->mfboOffset = buffer.pos() - 0x14;
                header->flags     |= 1; // set binary 1 to first bite
                buffer.write(MFBO.getChunk());
            }
            else
            {
                header->mfboOffset = 0;
                header->flags     &= ~1; // set binary 0 to first bite
            }

            if(MTFX.getSize())
            {
                header->mtfxOffset = buffer.pos() - 0x14;
                buffer.write(MTFX.getChunk());
            }
            else
                header->mtfxOffset = 0;

            file.write(buffer.data());

            buffer.close();
            file.close();

            return true;
        }

        return false;
    }

    void ADT::update()
    {

    }
}
