#include "globject.h"

GLObject::GLObject(QObject* parent)
: QObject(parent)
{

}

GLObject::~GLObject()
{
    delete shader;
}

void GLObject::Initialize()
{

}

void GLObject::Draw(const QMatrix4x4& mvp)
{
    Q_UNUSED(mvp);
}
