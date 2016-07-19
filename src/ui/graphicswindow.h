#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include "glwindow.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QVector>
#include <QPoint>

#include "../core/globject.h"
#include "../core/triangle.h"

class GraphicsWindow : public GLWindow, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    explicit GraphicsWindow(QWindow* parent = Q_NULLPTR);
    explicit GraphicsWindow(GraphicsWindow* view, QWindow* parent = Q_NULLPTR);
    ~GraphicsWindow();

    Triangle* getTriangle() const { return triangle; }
    const QVector<GLObject*> getGLObjects() { return glObjects; }
    const QMatrix4x4 getMVP() { return mvp; }

    void setMVP(const QMatrix4x4& modelViewProj) { mvp = modelViewProj; }

protected:
    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private:
    GraphicsWindow* mainView = Q_NULLPTR;

    QVector<GLObject*> glObjects;

    Triangle* triangle = Q_NULLPTR;

    QMatrix4x4 mvp;

    QPoint lastPos;
};

#endif // GRAPHICSWINDOW_H
