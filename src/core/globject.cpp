#include "GLObject.h"

GLObject::GLObject(QObject* parent)
: QObject(parent)
{

}

GLObject::~GLObject()
{
    delete shader;
}

void GLObject::Initialize(QOpenGLFunctions_4_5_Core* glFunctions)
{
    functions = glFunctions;
}

void GLObject::Draw(const QMatrix4x4& mvp)
{
    Q_UNUSED(mvp);
}
