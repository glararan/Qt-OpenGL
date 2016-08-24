#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include "GLWindow.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QVector>
#include <QPoint>

#include <Core/Camera.h>
#include <Core/Cube.h>
#include <Core/GLObject.h>
#include <Core/Triangle.h>

class GraphicsWindow : public GLWindow, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    explicit GraphicsWindow(QWindow* parent = Q_NULLPTR);
    explicit GraphicsWindow(GraphicsWindow* view, QWindow* parent = Q_NULLPTR);
    ~GraphicsWindow();

    Triangle* getTriangle() const { return triangle; }
    const QVector<GLObject*> getGLObjects() { return glObjects; }
    const Camera& getCamera() const { return camera; }

    //void setCamera(const Camera& cam) { camera = cam; }

protected:
    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

private:
    GraphicsWindow* mainView = Q_NULLPTR;

    Camera camera;

    QVector<GLObject*> glObjects;

    Triangle* triangle = Q_NULLPTR;
    Cube* cube = Q_NULLPTR;

    QPoint lastPos;

    QMatrix4x4 view;

    double frame = 0;
};

#endif // GRAPHICSWINDOW_H
