#ifndef WDTWIZARDPAGE2_H
#define WDTWIZARDPAGE2_H

#include <QWizardPage>
#include <QVector>
#include <QVarLengthArray>
#include <QPoint>

#include <WoW/ChunkHeaders.h>

#define adtBoolArray QVarLengthArray<QVarLengthArray<bool, MAP_SIZE>, MAP_SIZE>

namespace Ui
{
    class WdtWizardPage2;
}

class WdtWizardPage2 : public QWizardPage
{
    Q_OBJECT

public:
    explicit WdtWizardPage2(QWidget* parent = Q_NULLPTR);
    ~WdtWizardPage2();

    bool isComplete() const Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;

    adtBoolArray getAllCoords() const { return coords; }

private:
    Ui::WdtWizardPage2* ui;

    void setAllCoords(const bool& value);

    adtBoolArray coords;

private slots:
    void checkAll();
    void uncheckAll();

    void selectedCoords(const int& x, const int& y);
};

class MapSelector : public QWidget
{
    Q_OBJECT

public:
    explicit MapSelector(QWidget* parent = Q_NULLPTR);

    void setCoords(const adtBoolArray& array)
    {
        coords = array;

        QWidget::update();
    }

    adtBoolArray getCoords() const { return coords; }

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private:
    QVector<QPoint> selected;

    adtBoolArray coords;

signals:
    void selectedCoords(const int& x, const int& y);
};

#endif // WDTWIZARDPAGE2_H
