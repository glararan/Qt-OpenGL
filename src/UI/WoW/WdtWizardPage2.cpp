#include "WdtWizardPage2.h"
#include "ui_WdtWizardPage2.h"

#include <QtMath>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#define FIELD_SIZE 10

WdtWizardPage2::WdtWizardPage2(QWidget* parent)
: QWizardPage(parent)
, ui(new Ui::WdtWizardPage2)
{
    ui->setupUi(this);

    ui->mapView->resize(FIELD_SIZE * MAP_SIZE, FIELD_SIZE * MAP_SIZE);
    ui->mapView->setMinimumSize(ui->mapView->size());
    ui->mapView->setMaximumSize(ui->mapView->size());
    ui->scrollAreaWidgetContents->adjustSize();

    coords.resize(MAP_SIZE);

    for(QVarLengthArray<bool, MAP_SIZE>& array : coords)
        array.resize(MAP_SIZE);

    setAllCoords(false);
}

WdtWizardPage2::~WdtWizardPage2()
{
    delete ui;
}

void WdtWizardPage2::setAllCoords(const bool& value)
{
    for(int y = 0; y < MAP_SIZE; ++y)
    {
        for(int x = 0; x < MAP_SIZE; ++x)
            coords[x][y] = value;
    }

    ui->mapView->setCoords(coords);

    emit completeChanged();
}

void WdtWizardPage2::selectedCoords(const int& x, const int& y)
{
    coords[x][y] = !coords[x][y];

    emit completeChanged();
}

void WdtWizardPage2::checkAll()
{
    setAllCoords(true);
}

void WdtWizardPage2::uncheckAll()
{
    setAllCoords(false);
}

bool WdtWizardPage2::isComplete() const
{
    for(int y = 0; y < MAP_SIZE; ++y)
    {
        for(int x = 0; x < MAP_SIZE; ++x)
        {
            if(coords[x][y])
                return true;
        }
    }

    return false;
}

bool WdtWizardPage2::validatePage()
{
    return isComplete();
}

MapSelector::MapSelector(QWidget* parent)
: QWidget(parent)
{
    coords.resize(MAP_SIZE);

    for(QVarLengthArray<bool, MAP_SIZE>& array : coords)
        array.resize(MAP_SIZE);
}

void MapSelector::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::SolidPattern);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBackground(Qt::white);

    for(int y = 0; y < MAP_SIZE; ++y)
    {
        for(int x = 0; x < MAP_SIZE; ++x)
        {
            if(coords[x][y])
                painter.fillRect(x * FIELD_SIZE, y * FIELD_SIZE, FIELD_SIZE, FIELD_SIZE, QColor(0, 162, 232));

            painter.drawLine(x * FIELD_SIZE, 0, x * FIELD_SIZE, height());
        }

        painter.drawLine(0, y * FIELD_SIZE, width(), y * FIELD_SIZE);
    }

    painter.drawLine(0, height(), width(), height()); // last Y
    painter.drawLine(width(), 0, width(), height());  // last X
}

void MapSelector::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        const int x = qFloor((qreal)event->x() / (qreal)FIELD_SIZE);
        const int y = qFloor((qreal)event->y() / (qreal)FIELD_SIZE);

        if(!selected.contains(QPoint(x, y)))
        {
            selected.append(QPoint(x, y));

            coords[x][y] = !coords[x][y];

            emit selectedCoords(x, y);

            QWidget::update();
        }
    }

    QWidget::mouseMoveEvent(event);
}

void MapSelector::mouseReleaseEvent(QMouseEvent* event)
{
    if(!(event->buttons() & Qt::LeftButton))
        selected.clear();

    QWidget::mouseReleaseEvent(event);
}
