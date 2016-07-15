#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include "glwindow.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QVector>

#include "../core/globject.h"
#include "../core/triangle.h"

class GraphicsWindow : public GLWindow
{
    Q_OBJECT

public:
    GraphicsWindow();
    explicit GraphicsWindow(GraphicsWindow* view);
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
    GraphicsWindow* mainView;

    QVector<GLObject*> glObjects;

    Triangle* triangle;

    QMatrix4x4 mvp;
};

#endif // GRAPHICSWINDOW_H
