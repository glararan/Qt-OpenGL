#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#include "../ui/mainwindow.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char** argv);
    ~Application();

private:
    MainWindow* mainWindow;
};

#endif // APPLICATION_H
