#include "WdtWizardPage1.h"
#include "ui_WdtWizardPage1.h"

#include <WoW/Common.h>

#include <QFileDialog>
#include <QDir>

WdtWizardPage1::WdtWizardPage1(QWidget* parent)
: QWizardPage(parent)
, ui(new Ui::WdtWizardPage1)
{
    ui->setupUi(this);

    registerField("mapName*", ui->mapBox);
    registerField("wdtPath*", ui->pathBox);
    registerField("version*", ui->versionBox);
}

WdtWizardPage1::~WdtWizardPage1()
{
    delete ui;
}

void WdtWizardPage1::browsePath()
{
    ui->pathBox->setText(QFileDialog::getExistingDirectory(Q_NULLPTR, tr("Choose map folder"), QString(), QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly));
}

void WdtWizardPage1::versionChanged(const int version)
{
    int available = 0;

    switch((WoW::Version)version)
    {
        case WoW::Classic:
        case WoW::TBC:
            available = 1;
            break;

        case WoW::WotLK:
            available = 4;
            break;

        case WoW::Cataclysm:
            available = 7;
            break;

        case WoW::MoP:
        case WoW::WoD:
            available = 8;
            break;
            break;

        case WoW::Legion:
            available = 10;
            break;
    }

    // Disable items for that version
    for(int i = available; i < ui->flagsWidget->count(); ++i)
    {
        ui->flagsWidget->item(i)->setCheckState(Qt::Unchecked);
        ui->flagsWidget->item(i)->setHidden(true);
    }

    // Enable items for that version
    for(int i = 0; i < available; ++i)
    {
        if(ui->flagsWidget->item(i)->isHidden())
        {
            ui->flagsWidget->item(i)->setCheckState(Qt::Unchecked);
            ui->flagsWidget->item(i)->setHidden(false);
        }
    }
}

int WdtWizardPage1::getFlags() const // ugly way :/
{
    int flags = 0;

    if(ui->flagsWidget->item(0)->checkState() == Qt::Checked)
        flags += 0x1;

    if(ui->flagsWidget->item(1)->checkState() == Qt::Checked)
        flags += 0x2;

    if(ui->flagsWidget->item(2)->checkState() == Qt::Checked)
        flags += 0x4;

    if(ui->flagsWidget->item(3)->checkState() == Qt::Checked)
        flags += 0x8;

    if(ui->flagsWidget->item(4)->checkState() == Qt::Checked)
        flags += 0x10;

    if(ui->flagsWidget->item(5)->checkState() == Qt::Checked)
        flags += 0x20;

    if(ui->flagsWidget->item(6)->checkState() == Qt::Checked)
        flags += 0x40;

    if(ui->flagsWidget->item(7)->checkState() == Qt::Checked)
        flags += 0x80;

    if(ui->flagsWidget->item(8)->checkState() == Qt::Checked)
        flags += 0x100;

    if(ui->flagsWidget->item(9)->checkState() == Qt::Checked)
        flags += 0x8000;

    return flags;
}

bool WdtWizardPage1::isComplete() const
{
    return !ui->pathBox->text().isEmpty() && QDir(ui->pathBox->text()).exists();
}

bool WdtWizardPage1::validatePage()
{
    return isComplete();
}
