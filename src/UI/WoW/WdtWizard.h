#ifndef WDTWIZARD_H
#define WDTWIZARD_H

#include <QWizard>

namespace Ui
{
    class WdtWizard;
}

class WdtWizard : public QWizard
{
    Q_OBJECT

public:
    explicit WdtWizard(QWidget* parent = Q_NULLPTR);
    ~WdtWizard();

private:
    Ui::WdtWizard* ui;

public slots:
    void accept() Q_DECL_OVERRIDE;
};

#endif // WDTWIZARD_H
