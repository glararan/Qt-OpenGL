#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions_4_5_Core>

#include <QTimer>

class GLWindow : public QOpenGLWindow, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    GLWindow();
    explicit GLWindow(QOpenGLContext* sharedContext);
    ~GLWindow();

    void setFpsCounter(bool enable);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    bool event(QEvent* event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent* event) Q_DECL_OVERRIDE;

    virtual void initialize();
    virtual void render();

private:
    bool pendingUpdate = false;
    bool draw = true;

    bool painted = true;

    int fps = 0;

    QTimer fpsTimer;

public slots:
    void renderNow();

private slots:
    void fpsTimeout();
};

#endif // GLWINDOW_H
