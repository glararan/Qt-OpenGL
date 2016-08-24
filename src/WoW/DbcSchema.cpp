#include "DbcSchema.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QDomDocument>

DbcSchema::DbcSchema()
{

}

DbcSchema::DbcSchema(const QString& fileName, const int& specifiedColumns)
{
    load(fileName, specifiedColumns);
}

void DbcSchema::load(const QString& fileName, const int& specifiedColumns)
{
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
    {
        // throw
    }

    QDomDocument document;

    if(!document.setContent(&file))
    {
        // throw
    }

    file.close();

    QDomElement root = document.documentElement();

    QDomNode column = root.firstChild();

    columns.clear();

    while(!column.isNull())
    {
        QDomElement element = column.toElement();

        ColumnSchema schema;
        schema.position = element.attribute("Position").toInt();
        schema.width    = element.attribute("Width").toInt();
        schema.name     = element.attribute("Name");
        schema.type     = (ColumnType)element.attribute("Type").toInt();

        columns.append(schema);

        column = column.nextSibling();
    }

    while(columnCount > specifiedColumns)
        columns.removeAt(columnCount - 1);

    while(columns.count() < specifiedColumns)
    {
        ColumnSchema schema;
        schema.position = columnCount;
        schema.name = "Column" + columnCount;
        schema.type = Int32;
        schema.width = 100;

        columns.append(schema);
    }
}

void DbcSchema::save(const QString& fileName, const QString& dbcName)
{
    for(ColumnSchema& schema : columns)
    {
        if(schema.width == 0)
            schema.width = 100;
    }

    QFile file(fileName);

    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text))
    {
        // throw
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    writer.writeStartElement("DbcSchema");
    writer.writeAttribute("Version", "1.0");
    writer.writeAttribute("Target", dbcName);

    for(ColumnSchema& column : columns)
    {
        writer.writeStartElement("Column");
        writer.writeAttribute("Name", column.name);
        writer.writeAttribute("Position", QString::number(column.position));
        writer.writeAttribute("Type", QString::number((int)column.type));
        writer.writeAttribute("Width", QString::number(column.width));
        writer.writeEndElement(); // Column
    }

    writer.writeEndElement(); // DbcSchema
    writer.writeEndDocument();

    file.close();
}
