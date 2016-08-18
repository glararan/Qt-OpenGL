#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QObject>

#include <QVector>
#include <QOpenGLShaderProgram>

#include "Shader.h"

class ShaderManager : public QObject
{
    Q_OBJECT

public:
    explicit ShaderManager(QObject* parent = 0);
    ~ShaderManager();

    QOpenGLShaderProgram* getShader(const QVector<ShaderDetails>& details);

private:
    QVector<Shader> shaders;
};

#endif // SHADERMANAGER_H
