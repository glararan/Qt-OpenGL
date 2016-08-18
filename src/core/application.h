#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#include <UI/MainWindow.h>

#include "Global.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char** argv);
    ~Application();

private:
    MainWindow* mainWindow;

    GLManager* glMgr;
};

#endif // APPLICATION_H
