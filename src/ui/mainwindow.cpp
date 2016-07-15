#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, graphics(new GraphicsWindow())
{
    ui->setupUi(this);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);
    format.setSamples(16);
    format.setSwapInterval(0);

    graphics->setFormat(format);
    graphicsWidget = QWidget::createWindowContainer(graphics, this);

    setCentralWidget(graphicsWidget);

    graphics2 = new GraphicsWindow(graphics);
    graphics2->setFormat(format);
    graphics2->resize(800, 600);
    graphics2->show();
}

MainWindow::~MainWindow()
{
    delete graphicsWidget;
    delete graphics;
    delete graphics2;
    delete ui;
}
