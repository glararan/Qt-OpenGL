#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QObject>

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>

class GLObject : public QObject
{
    Q_OBJECT

public:
    explicit GLObject(QObject* parent = 0);
    ~GLObject();

    virtual void Initialize(QOpenGLFunctions_4_5_Core* glFunctions);
    virtual void Draw(const QMatrix4x4& mvp);

    const QOpenGLShaderProgram* getShader() { return shader; }

protected:
    QOpenGLFunctions_4_5_Core* functions;

    QOpenGLShaderProgram* shader;

    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

signals:

public slots:
};

#endif // GLOBJECT_H
