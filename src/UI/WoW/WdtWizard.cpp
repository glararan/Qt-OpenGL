#include "WdtWizard.h"
#include "ui_WdtWizard.h"

#include "WdtWizardPage1.h"
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
    const QString wdtFile(QString("%1/%2.wdt").arg(wdtPath).arg(mapName));

    adtBoolArray adtCoords(ui->page2->getAllCoords());

    WoW::Version version((WoW::Version)field("version").toInt());

    // Check for existing wdt.. if exists ask what to do
    if(QFile::exists(wdtFile))
    {
        if(QMessageBox::question(Q_NULLPTR, "File already exists", "Do you want to replace file?", QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Cancel)
            return;
    }

    // Create WDT & ADT
    WoW::WDT wdt; // todo set version
    WoW::ADT adt; // todo set version

    // Get only used coords from large array
    for(int y = 0; y < MAP_SIZE; ++y)
    {
        for(int x = 0; x < MAP_SIZE; ++x)
        {
            if(adtCoords[x][y])
            {
                wdt.setADT(WoW::WDT::MapExists, y, x); // Y & X are reversed
                //adt.save(QString("%1/%2_%3_%4.adt").arg(wdtPath).arg(mapName).arg(x).arg(y));
            }
        }
    }

    wdt.setFlags(ui->page1->getFlags());
    wdt.save(wdtFile);

    QDialog::accept();

    qDebug() << "WdtWizard successfully created a new map";
}
