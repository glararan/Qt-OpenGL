#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "WoW/WdtWizard.h"

#include <Core/Global.h>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graphics = new GraphicsWindow();

    graphicsWidget = QWidget::createWindowContainer(graphics, this);
    graphicsWidget->setFocusPolicy(Qt::StrongFocus);

    setCentralWidget(graphicsWidget);

    /*graphics2 = new GraphicsWindow(graphics);
    graphics2->resize(800, 600);

    QTimer::singleShot(2000, this, [this]()
    {
        graphics2->show();
    });*/
}

MainWindow::~MainWindow()
{
    delete graphicsWidget;
    delete graphics;
    delete graphics2;
    delete ui;
}

void MainWindow::showNewMapWizard()
{
    WdtWizard* wizard(new WdtWizard());
    wizard->show();

    connect(this, &MainWindow::destroyed, wizard, &WdtWizard::deleteLater);
}
