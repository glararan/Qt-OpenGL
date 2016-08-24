#ifndef DBC_H
#define DBC_H

#include <QObject>
#include <QHash>
#include <QFile>
#include <QVector>

#include "DbcSchema.h"

#define HeaderSizeDbc 20
#define HeaderSizeDb2 48

class QTableWidget;
class QTableWidgetItem;

namespace WoW
{
    class DBC
    {
    public:
        enum FileFormat
        {
            Dbc,
            Db2,
            AdbCache
        };

        struct Record
        {
            qint32 position;
            qint32 index;

            Record(const qint32& pos = 0, const qint32& id = 0)
            : position(pos)
            , index(id)
            {
            }
        };

        DBC();
        explicit DBC(const QString& fileName);

        qint32 addStringToTable(const QString& value);

        QString getString(const int& stringTablePosition);
        QString getString(const int& record, const int& column);
        int     getInt32(const int& record, const int& column);
        uint    getUInt32(const int& record, const int& column);
        float   getFloat(const int& record, const int& column);

        QVector<Record> getRecords();

        void write(const QString& destination, const DbcSchema& schema, QTableWidget* view);

    private:
        FileFormat format = Dbc;

        QFile file;

        QHash<qint32, QString> stringTable;

        qint32 magic;
        qint32 count;
        qint32 recordSize;
        qint32 perRecord;
        qint32 stringBlockSize;
        qint32 stringBlockOffset;

        qint32 headerLength;
        qint32 lookupID;

        qint32 hashTable;
        qint32 build;
        qint32 lastWrittenTimestamp;
        qint32 minID;
        qint32 maxID;
        qint32 locale;
        qint32 unk;
    };
}

#endif // DBC_H
