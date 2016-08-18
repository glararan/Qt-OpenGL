#ifndef SHADER_H
#define SHADER_H

#include <QObject>

#include <QOpenGLShaderProgram>
#include <QSharedPointer>

class ShaderDetails
{
public:
    explicit ShaderDetails(const QOpenGLShader::ShaderType shaderType = QOpenGLShader::Vertex, const QString& shaderFile = QString());

    QString getFile() const                   { return file; }
    QOpenGLShader::ShaderType getType() const { return type; }

    bool operator==(const ShaderDetails& other) const;

private:
    QString file;
    QOpenGLShader::ShaderType type;
};

class Shader
{
public:
    void addDetail(const QString& file, const QOpenGLShader::ShaderType& type);

    bool contains(const ShaderDetails& detail) const;

    QOpenGLShaderProgram* getProgram() { return program.data(); }

private:
    QVector<ShaderDetails> details;
    QSharedPointer<QOpenGLShaderProgram> program;
};

#endif // SHADER_H
