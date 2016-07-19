#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../core/global.h"

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graphics = new GraphicsWindow();

    graphicsWidget = QWidget::createWindowContainer(graphics, this);

    setCentralWidget(graphicsWidget);

    graphics2 = new GraphicsWindow(graphics);
    graphics2->resize(800, 600);

    QTimer::singleShot(2000, this, [this]()
    {
        graphics2->show();
    });
}

MainWindow::~MainWindow()
{
    delete graphicsWidget;
    delete graphics;
    delete graphics2;
    delete ui;
}
