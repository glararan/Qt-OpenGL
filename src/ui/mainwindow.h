#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "GraphicsWindow.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow* ui;

    GraphicsWindow* graphics;
    GraphicsWindow* graphics2;
    QWidget* graphicsWidget;
};

#endif // MAINWINDOW_H
