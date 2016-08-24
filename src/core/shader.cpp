#include "Shader.h"

ShaderDetails::ShaderDetails(const QOpenGLShader::ShaderType shaderType, const QString& shaderFile)
: file(shaderFile)
, type(shaderType)
{
}

bool ShaderDetails::operator==(const ShaderDetails& other) const
{
    if(file == other.file && type == other.type)
        return true;

    return false;
}

/// Shader
Shader::Shader() : program(QSharedPointer<QOpenGLShaderProgram>(new QOpenGLShaderProgram()))
{
}

Shader::~Shader()
{
    program.clear();
    details.clear();
}

void Shader::addDetail(const QString& file, const QOpenGLShader::ShaderType& type)
{
    details.append(ShaderDetails(type, file));
}

bool Shader::contains(const ShaderDetails& detail) const
{
    for(const ShaderDetails& shaderDetail: details)
    {
        if(shaderDetail == detail)
            return true;
    }

    return false;
}
