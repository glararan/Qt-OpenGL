#include "triangle.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_5_Core>

const GLfloat vertices[] =
{
    0.0f, 0.707f,
    -0.5f, -0.5f,
    0.5f, -0.5f
};

const GLfloat colors[] =
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
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

void Triangle::Initialize()
{
    // todo shader manager, borrow if exists
    shader = new QOpenGLShaderProgram(this);
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "F:\\Work Library\\Programming\\Qt\\Unnamed\\bin\\shaders\\basic\\triangle.vert");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "F:\\Work Library\\Programming\\Qt\\Unnamed\\bin\\shaders\\basic\\triangle.frag");
    shader->link();
    shader->bind();

    mvpLoc = shader->uniformLocation("mvp");

    // VBO & VAO
    /*glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(6 * sizeof(GLfloat) + 9 * sizeof(GLfloat));
    vbo.write(0, vertices, 6 * sizeof(GLfloat));
    vbo.write(6 * sizeof(GLfloat), colors, 9 * sizeof(GLfloat));

    vao.create();
    vao.bind();

    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
    shader->setAttributeBuffer(1, GL_FLOAT, 6 * sizeof(GLfloat), 3, 0);

    vao.release();
    vbo.release();
    shader->release();
}

void Triangle::Draw(const QMatrix4x4& mvp)
{
    QOpenGLFunctions_4_5_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();

    shader->bind();
    {
        shader->setUniformValue(mvpLoc, mvp);

        //glBindVertexArray(vao);
        vao.bind();
        functions->glDrawArrays(GL_TRIANGLES, 0, 3);
        vao.release();

        //qDebug() << "Draw";
    }
    shader->release();
}
