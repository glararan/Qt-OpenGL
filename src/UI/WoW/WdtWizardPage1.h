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

    bool isComplete() const Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;

private:
    Ui::WdtWizardPage1* ui;

private slots:
    void browsePath();
};

#endif // WDTWIZARDPAGE1_H
