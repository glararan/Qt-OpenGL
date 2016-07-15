#include "application.h"

#include "../ui/graphicswindow.h"

Application::Application(int argc, char** argv)
: QApplication(argc, argv)
{
    setApplicationName("Unnamed");
    setApplicationVersion("0.0.0");
    setOrganizationDomain("http://veteska.cz");
    setOrganizationName("Veteška");

    mainWindow = new MainWindow();
    mainWindow->show();
}

Application::~Application()
{
    delete mainWindow;
}
