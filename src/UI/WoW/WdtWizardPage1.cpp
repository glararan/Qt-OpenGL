#include "WdtWizardPage1.h"
#include "ui_WdtWizardPage1.h"

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
    ui->pathBox->setText(QFileDialog::getExistingDirectory(Q_NULLPTR, tr("Choose map folder")));
}

bool WdtWizardPage1::isComplete() const
{
    return !ui->pathBox->text().isEmpty() && QDir(ui->pathBox->text()).exists();
}

bool WdtWizardPage1::validatePage()
{
    return isComplete();
}
