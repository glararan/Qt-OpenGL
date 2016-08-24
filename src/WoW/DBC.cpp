#include "DBC.h"

#include <QFile>
#include <QTableWidget>

/* do somethings with this! create utilities or something! */
bool hasRowEmptyCells(QTableWidget* view, const int& row)
{
    for(int x = 0; x < view->columnCount(); ++x)
    {
        if(!view->takeItem(row, x)->text().isEmpty())
            return false;
    }

    return true;
}

namespace WoW
{
    DBC::DBC()
    {
    }

    DBC::DBC(const QString& fileName)
    {
        file.setFileName(fileName);

        if(!file.open(QIODevice::ReadOnly))
        {
            // throw;
        }

        file.seek(0);

        magic = file.read(sizeof(qint32)).toInt();

        switch(magic)
        {
            case 0x43424457: // Dbc
                {
                    format       = Dbc;
                    headerLength = HeaderSizeDbc;
                }
                break;

            case 0x32424457: // Db2
                {
                    format       = Db2;
                    headerLength = HeaderSizeDb2;
                }
                break;

            case 0x32484357: // AdbCache
                {
                    format       = AdbCache;
                    headerLength = HeaderSizeDb2;
                }
                break;

            default: // Corrupted
                {
                    // throw;
                }
                break;
        }

        count           = file.read(sizeof(qint32)).toInt();
        recordSize      = file.read(sizeof(qint32)).toInt();
        perRecord       = file.read(sizeof(qint32)).toInt();
        stringBlockSize = file.read(sizeof(qint32)).toInt();

        if(format != Dbc)
        {
            hashTable            = file.read(sizeof(qint32)).toInt();
            build                = file.read(sizeof(qint32)).toInt();
            lastWrittenTimestamp = file.read(sizeof(qint32)).toInt();
            minID                = file.read(sizeof(qint32)).toInt();
            maxID                = file.read(sizeof(qint32)).toInt();
            locale               = file.read(sizeof(qint32)).toInt();
            unk                  = file.read(sizeof(qint32)).toInt();

            if (maxID != 0)
            {
                lookupID = headerLength;

                headerLength = headerLength + (maxID - minID + 1) * 6;
            }
        }
        else
        {
            hashTable            = 0;
            build                = -1;
            lastWrittenTimestamp = 0;
            minID                = -1;
            maxID                = -1;
            locale               = 0;
        }

        stringBlockOffset = perRecord * count + headerLength;
    }

    qint32 DBC::addStringToTable(const QString& value)
    {
        for(const QString& string : stringTable.values())
        {
            if(string == value)
                return stringTable.key(string);
        }

        qint32 position = stringTable.count() == 0 ? 0 : stringTable.end().key() + stringTable.end().value().toUtf8().count() + 1;

        stringTable.insert(position, value);

        return position;
    }

    QString DBC::getString(const int& stringTablePosition)
    {
        qint64 currentPos = file.pos();

        file.seek(stringBlockOffset + stringTablePosition);

        qint32 length = 0;

        while(file.read(sizeof(qint8)).toInt() != 0)
            length++;

        file.seek(stringBlockOffset + stringTablePosition);

        QByteArray temp = file.read(length);

        file.seek(currentPos);

        return temp; // maybe QString::fromUtf8
    }

    QString DBC::getString(const int& record, const int& column)
    {
        return getString(getInt32(record, column));
    }

    int DBC::getInt32(const int& record, const int& column)
    {
        file.seek(record * perRecord + headerLength + column * 4);

        return file.read(sizeof(qint32)).toInt();
    }

    uint DBC::getUInt32(const int &record, const int &column)
    {
        file.seek(record * perRecord + headerLength + column * 4);

        return file.read(sizeof(quint32)).toUInt();
    }

    float DBC::getFloat(const int& record, const int& column)
    {
        file.seek(record * perRecord + headerLength + column * 4);

        return file.read(sizeof(float)).toFloat();
    }

    QVector<DBC::Record> DBC::getRecords()
    {
        QVector<Record> records;

        qint32 currentPos = headerLength;

        for(int i = 0; i < count; ++i)
        {
            records.append(Record(currentPos, i));

            currentPos += recordSize * 4;
        }

        return records;
    }

    void DBC::write(const QString& destination, const DbcSchema& schema, QTableWidget* view)
    {
        QVector<int> rows;

        for(int y = 0; y < view->rowCount(); ++y)
        {
            if(y == view->rowCount() - 1 && hasRowEmptyCells(view, y))
                continue;

            rows.append(y);
        }

        QFile writer(destination);

        if(!writer.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            // throw
        }

        writer.write(QByteArray::number(magic));
        writer.write(QByteArray::number(rows.count()));
        writer.write(QByteArray::number(view->columnCount()));
        writer.write(QByteArray::number(view->columnCount() * 4));
        writer.write(QByteArray::number(0));

        if(format != Dbc)
        {
            writer.write(QByteArray::number(hashTable));
            writer.write(QByteArray::number(build));
            writer.write(QByteArray::number(lastWrittenTimestamp));
            writer.write(QByteArray::number(minID));
            writer.write(QByteArray::number(maxID));
            writer.write(QByteArray::number(locale));
            writer.write(QByteArray::number(unk));
        }

        QVector<DbcSchema::ColumnSchema> columnSchema = schema.getColumns();

        for(const int& row : rows)
        {
            for(int x = 0; x < recordSize; ++x)
            {
                switch(columnSchema.at(x).type)
                {
                case DbcSchema::Int32:
                case DbcSchema::Boolean:
                    writer.write(QByteArray::number(view->takeItem(row, x)->data(0).toInt()));
                    break;

                case DbcSchema::Int32Flags:
                    writer.write(QByteArray::number(view->takeItem(row, x)->data(0).toInt()));
                    break;

                case DbcSchema::Float:
                    writer.write(QByteArray::number(view->takeItem(row, x)->data(0).toFloat()));
                    break;

                case DbcSchema::String:
                    writer.write(QByteArray::number(addStringToTable(view->takeItem(row, x)->data(0).toString())));
                    break;

                case DbcSchema::UInt32:
                    writer.write(QByteArray::number(view->takeItem(row, x)->data(0).toUInt()));
                    break;
                }
            }
        }

        // stringTable
        for(QString str : stringTable.values())
        {
            writer.write(str.toUtf8());
            writer.write(QByteArray::number(0));
        }

        writer.seek(16);

        if (stringTable.count() > 0)
            writer.write(QByteArray::number(stringTable.end().key() + stringTable.end().value().toUtf8().count() + 1));

        writer.close();
    }
}
