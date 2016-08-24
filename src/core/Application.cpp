#include "Application.h"

#include <UI/GraphicsWindow.h>

Application::Application(int argc, char** argv)
: QApplication(argc, argv)
{
    setApplicationName("Unnamed");
    setApplicationVersion("0.0.0");
    setOrganizationDomain("http://veteska.cz");
    setOrganizationName("Veteška");

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);
    format.setSamples(16);
    format.setSwapInterval(0);

    QSurfaceFormat::setDefaultFormat(format);

    glMgr = GLMgr;

    mainWindow = new MainWindow();
    mainWindow->show();
}

Application::~Application()
{
    delete mainWindow;
}
