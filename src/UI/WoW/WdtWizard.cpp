#include "WdtWizard.h"
#include "ui_WdtWizard.h"

#include "WdtWizardPage2.h"

#include <WoW/Common.h>
#include <WoW/ADT.h>
#include <WoW/WDT.h>

#include <QFile>
#include <QMessageBox>
#include <QVector>
#include <QPoint>
#include <QDebug>

WdtWizard::WdtWizard(QWidget* parent)
: QWizard(parent)
, ui(new Ui::WdtWizard)
{
    ui->setupUi(this);
}

WdtWizard::~WdtWizard()
{
    delete ui;
}

void WdtWizard::accept()
{
    // Get fields
    const QString mapName(field("mapName").toString());
    const QString wdtPath(field("wdtPath").toString());

    adtBoolArray adtCoords(ui->page2->getAllCoords());

    WoW::Version version((WoW::Version)field("version").toInt());

    // Check for existing wdt.. if exists ask what to do
    if(QFile::exists(wdtPath))
    {
        if(QMessageBox::question(Q_NULLPTR, "File already exists", "Do you want to replace file?", QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Cancel)
            return;
    }

    // Create WDT & ADT
    WoW::WDT wdt; // todo set version
    WoW::ADT adt; // todo set version

    // Get only used coords from large array
    QVector<QPoint> coords;

    for(int y = 0; y < MAP_SIZE; ++y)
    {
        for(int x = 0; x < MAP_SIZE; ++x)
        {
            if(adtCoords[x][y])
                coords.append(QPoint(x, y));
        }
    }

    // Use only used coords from large array
    for(const QPoint& coord : coords)
    {
        wdt.setADT(1, coord.x(), coord.y());
        adt.save(QString("%1/%2_%3_%4.adt").arg(wdtPath).arg(mapName).arg(coord.x()).arg(coord.y()));
    }

    wdt.save(QString("%1/%2.wdt").arg(wdtPath).arg(mapName));

    QDialog::accept();
}
