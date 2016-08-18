#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QPaintDeviceWindow>

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_5_Core>
#include <QTimer>
#include <QElapsedTimer>

class GLWindowPrivate;

class GLWindow : public QPaintDeviceWindow
{
    Q_OBJECT

public:
    enum UpdateBehavior
    {
        NoPartialUpdate,
        PartialUpdateBlit,
        PartialUpdateBlend
    };

    explicit GLWindow(UpdateBehavior updateBehavior, QWindow* parent = Q_NULLPTR);
    explicit GLWindow(QOpenGLContext* shareContext, UpdateBehavior updateBehavior, QWindow* parent = Q_NULLPTR);
    explicit GLWindow(QOpenGLFunctions_4_5_Core* glFunctions, QOpenGLContext* shareContext, UpdateBehavior updateBehavior, QWindow* parent = Q_NULLPTR);
    ~GLWindow();

    bool isValid() const;

    void makeCurrent();
    void doneCurrent();

    void setFpsCounter(bool enable);

    QOpenGLContext* context() const;

    UpdateBehavior updateBehavior() const;
    GLuint defaultFramebufferObject() const;

    QImage grabFramebuffer();

    const double& getDeltaTime() const { return deltaTime; }

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void paintUnderGL();
    virtual void paintOverGL();
    virtual void resizeGL(int width, int height);

    bool event(QEvent* event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent* event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

    int metric(PaintDeviceMetric metric) const Q_DECL_OVERRIDE;

    QPaintDevice* redirected(QPoint*) const Q_DECL_OVERRIDE;

    virtual void initialize();
    virtual void render();

private:
    Q_DECLARE_PRIVATE(GLWindow)
    Q_DISABLE_COPY(GLWindow)

    QOpenGLContext* glContext = Q_NULLPTR;
    QOpenGLFunctions_4_5_Core* functions = Q_NULLPTR;

    bool pendingUpdate = false;
    bool draw = true;
    bool isCopy = false;

    bool painted = true;

    double deltaTime = 0;
    double lastFrame = 0;

    int fps = 0;

    QTimer fpsTimer;
    QElapsedTimer deltaTimer;

signals:
    void frameSwapped();

public slots:
    void renderNow();

private slots:
    void fpsTimeout();
};

#endif // GLWINDOW_H
