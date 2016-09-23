#ifndef DBCSCHEMA_H
#define DBCSCHEMA_H

#include <QObject>
#include <QVector>

class DbcSchema
{
public:
    enum ColumnType
    {
        Int32,
        Float,
        String,
        Int32Flags,
        Boolean,
        UInt32
    };

    struct ColumnSchema
    {
        qint32 position;
        qint32 width;

        QString name;

        ColumnType type;
    };

    DbcSchema();
    explicit DbcSchema(const QString& fileName, const int& specifiedColumns);

    void addColumn(const ColumnSchema& column) { columns.append(column); }

    QVector<ColumnSchema> getColumns() const { return columns; }
    qint32 getColumnsCount() const { return columnCount; }

    void setColumnsCount(const qint32& count) { columnCount = count; }

    void load(const QString& fileName, const int& specifiedColumns);
    void save(const QString& fileName, const QString& dbcName);

    ColumnSchema const& operator[](int index) const { return columns.at(index); }

    // please find another way!!!
    static ColumnType getColumnType(const qint32 value) { Q_UNUSED(value); return Int32; }
    static ColumnType getColumnType(const float value) { Q_UNUSED(value); return Float; }
    static ColumnType getColumnType(const QString value) { Q_UNUSED(value); return String; }
    static ColumnType getColumnType(const bool value) { Q_UNUSED(value); return Boolean; }
    static ColumnType getColumnType(const quint32 value) { Q_UNUSED(value); return UInt32; }

    template<typename T>
    static ColumnType getColumnType(const T value) { Q_UNUSED(value); return Int32Flags; }

private:
    QVector<ColumnSchema> columns;

    qint32 columnCount;
};

#endif // DBCSCHEMA_H
