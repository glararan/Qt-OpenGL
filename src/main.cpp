#include "core/application.h"

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    Application app(argc, argv);

    return app.exec();
}
