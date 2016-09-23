#ifndef WDTWIZARDPAGE1_H
#define WDTWIZARDPAGE1_H

#include <QWizardPage>

namespace Ui
{
    class WdtWizardPage1;
}

class WdtWizardPage1 : public QWizardPage
{
    Q_OBJECT

public:
    explicit WdtWizardPage1(QWidget* parent = Q_NULLPTR);
    ~WdtWizardPage1();

    int getFlags() const;

    bool isComplete() const Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;

private:
    Ui::WdtWizardPage1* ui;

private slots:
    void browsePath();
    void versionChanged(const int version);
};

#endif // WDTWIZARDPAGE1_H
