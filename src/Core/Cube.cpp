#include "Cube.h"

#include "Vertex.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_5_Core>

// Front Verticies
Vertex frontTopRight(QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 0.0f, 0.0f));
Vertex frontTopLeft (QVector3D( 0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f));
Vertex frontBotLeft (QVector3D( 0.5f,  0.5f, -0.5f), QVector3D(0.0f, 0.0f, 1.0f));
Vertex frontBotRight(QVector3D(-0.5f,  0.5f, -0.5f), QVector3D(0.0f, 0.0f, 0.0f));

// Back Verticies
Vertex backTopRight(QVector3D(-0.5f, -0.5f,  0.5f), QVector3D(1.0f, 1.0f, 0.0f));
Vertex backTopLeft (QVector3D( 0.5f, -0.5f,  0.5f), QVector3D(0.0f, 1.0f, 1.0f));
Vertex backBotLeft (QVector3D( 0.5f,  0.5f,  0.5f), QVector3D(1.0f, 0.0f, 1.0f));
Vertex backBotRight(QVector3D(-0.5f,  0.5f,  0.5f), QVector3D(1.0f, 1.0f, 1.0f));

const Vertex vertices[] =
{
    // Face 1 (Front)
    frontTopRight, frontTopLeft,  frontBotLeft,
    frontBotLeft,  frontBotRight, frontTopRight,
    // Face 2 (Back)
    backBotRight, backTopLeft,  backTopRight,
    backTopLeft,  backBotRight, backBotLeft,
    // Face 3 (Top)
    frontTopRight, backTopRight, backTopLeft,
    backTopLeft,   frontTopLeft, frontTopRight,
    // Face 4 (Bottom)
    frontBotRight, frontBotLeft, backBotLeft,
    backBotLeft,   backBotRight, frontBotRight,
    // Face 5 (Left)
    frontBotLeft, frontTopLeft, backTopLeft,
    frontBotLeft, backTopLeft,  backBotLeft,
    // Face 6 (Right)
    frontTopRight, frontBotRight, backBotRight,
    backBotRight,  backTopRight,  frontTopRight
};

Cube::Cube(QObject* parent)
: GLObject(parent)
{

}

Cube::~Cube()
{
    vao.destroy();
    vbo.destroy();

    delete shader;
}

void Cube::Initialize(QOpenGLFunctions_4_5_Core* glFunctions)
{
    GLObject::Initialize(glFunctions);

    // todo shader manager, borrow if exists
    shader = new QOpenGLShaderProgram(this);
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "F:\\Work Library\\Programming\\Qt\\Unnamed\\bin\\shaders\\basic\\cube.vert");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "F:\\Work Library\\Programming\\Qt\\Unnamed\\bin\\shaders\\basic\\triangle.frag");
    shader->link();
    shader->bind();

    modelLoc = shader->uniformLocation("model");
    viewLoc = shader->uniformLocation("view");
    projectionLoc = shader->uniformLocation("projection");

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

    // Translation
    model.setToIdentity();
    model.translate(0.0f, 0.0f, 0.0f);
    model.rotate(0.0f, 1.0f, 0.3f, 0.5f);
}

void Cube::Draw(const QMatrix4x4& mvp)
{
    shader->bind();
    {
        shader->setUniformValue("mvp", mvp);

        vao.bind();
        functions->glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
        vao.release();
    }
    shader->release();
}

void Cube::Draw2(const QMatrix4x4& view, const QMatrix4x4& proj)
{
    shader->bind();
    {
        shader->setUniformValue(modelLoc, model);
        shader->setUniformValue(viewLoc, view);
        shader->setUniformValue(projectionLoc, proj);

        vao.bind();
        functions->glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
        vao.release();
    }
    shader->release();
}
