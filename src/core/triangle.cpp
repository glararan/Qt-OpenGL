#include "Triangle.h"

#include "Vertex.h"
#include "Global.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_5_Core>

const Vertex vertices[] =
{
    Vertex(QVector3D(0.0f, 0.707f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)),
    Vertex(QVector3D(-0.5f, -0.5f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)),
    Vertex(QVector3D(0.5f, -0.5f,  0.0f), QVector3D(0.0f, 0.0f, 1.0f))
};

Triangle::Triangle(QObject* parent)
: GLObject(parent)
{

}

Triangle::~Triangle()
{
    vao.destroy();
    vbo.destroy();

    delete shader;
}

void Triangle::Initialize(QOpenGLFunctions_4_5_Core* glFunctions)
{
    GLObject::Initialize(glFunctions);

    // todo shader manager, borrow if exists
    /*shader = new QOpenGLShaderProgram(this);
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "F:\\Work Library\\Programming\\Qt\\Unnamed\\bin\\shaders\\basic\\triangle.vert");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "F:\\Work Library\\Programming\\Qt\\Unnamed\\bin\\shaders\\basic\\triangle.frag");
    shader->link();
    shader->bind();*/

    QVector<ShaderDetails> shaderDetails;
    shaderDetails << ShaderDetails(QOpenGLShader::Vertex,   "F:\\Work Library\\Programming\\Qt\\Unnamed\\bin\\shaders\\basic\\triangle.vert");
    shaderDetails << ShaderDetails(QOpenGLShader::Fragment, "F:\\Work Library\\Programming\\Qt\\Unnamed\\bin\\shaders\\basic\\triangle.frag");

    shader = ShaderMgr->getShader(shaderDetails);
    shader->bind();

    mvpLoc = shader->uniformLocation("mvp");

    // VBO & VAO
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(vertices, sizeof(vertices));

    vao.create();
    vao.bind();

    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionSize, Vertex::stride());
    shader->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(),    Vertex::ColorSize,    Vertex::stride());

    vao.release();
    vbo.release();
    shader->release();
}

void Triangle::Draw(const QMatrix4x4& mvp)
{
    shader->bind();
    {
        shader->setUniformValue(mvpLoc, mvp);

        //glBindVertexArray(vao);
        vao.bind();
        functions->glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
        vao.release();
    }
    shader->release();
}
