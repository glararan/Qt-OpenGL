#include "ShaderManager.h"

ShaderManager::ShaderManager(QObject* parent)
: QObject(parent)
{
}

ShaderManager::~ShaderManager()
{
}

QOpenGLShaderProgram* ShaderManager::getShader(const QVector<ShaderDetails>& details)
{
    // return that shader contains all details otherwise create a new one
    for(Shader& shader: shaders)
    {
        for(const ShaderDetails& detail: details)
        {
            if(!shader.contains(detail))
                break;
        }

        return shader.getProgram();
    }

    // no suitable shader founded, lets create a new one
    Shader shader;

    for(const ShaderDetails detail: details)
    {
        shader.addDetail(detail.getFile(), detail.getType());

        shader.getProgram()->setParent(this);

        if(!shader.getProgram()->addShaderFromSourceFile(detail.getType(), detail.getFile()))
            qDebug() << tr("Failed to add shader type %1 from file %2").arg(detail.getType()).arg(detail.getFile());
    }

    if(!shader.getProgram()->link())
        qDebug() << tr("Failed to link shader");

    shaders.append(shader);

    return shader.getProgram();
}
