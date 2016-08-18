#include "GLManager.h"

#include <QDebug>

GLManager::GLManager(QObject* parent)
: QObject(parent)
{
    glContext = new QOpenGLContext(this);
    glContext->setFormat(QSurfaceFormat::defaultFormat());
    glContext->create();

    qDebug() << "OpenGL Context initialization:" << (glContext->isValid() ? "success" : "failed");
}

GLManager::~GLManager()
{
    delete glContext;
}
