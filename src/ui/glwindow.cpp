#include "glwindow.h"

#include <QtGui/private/qpaintdevicewindow_p.h>
#include <QtGui/private/qopengltextureblitter_p.h>
#include <QtGui/private/qopenglextensions_p.h>

#include <QOpenGLPaintDevice>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QImage>
#include <QOffscreenSurface>
#include <QDebug>

/// PaintDevice
class GLWindowPaintDevice : public QOpenGLPaintDevice
{
public:
    GLWindowPaintDevice(GLWindow* window)
    : win(window)
    {
    }

    void ensureActiveTarget() Q_DECL_OVERRIDE;

    GLWindow* win;
};

/// Private Window
class GLWindowPrivate : public QPaintDeviceWindowPrivate
{
    Q_DECLARE_PUBLIC(GLWindow)

public:
    GLWindowPrivate(QOpenGLContext* glContext, GLWindow::UpdateBehavior updateBehavior)
    : updateBehavior(updateBehavior)
    , hasFboBlit(false)
    , shareContext(glContext)
    {
        /*if (!shareContext)
            this->shareContext = qt_gl_global_share_context();*/
    }

    ~GLWindowPrivate();

    static GLWindowPrivate* get(GLWindow* window) { return window->d_func(); }

    void bindFBO();
    void initialize();

    void beginPaint(const QRegion& region) Q_DECL_OVERRIDE;
    void endPaint() Q_DECL_OVERRIDE;
    void flush(const QRegion& region) Q_DECL_OVERRIDE;

    GLWindow::UpdateBehavior updateBehavior;
    bool hasFboBlit;
    QScopedPointer<QOpenGLContext> context;
    QOpenGLContext* shareContext;
    QScopedPointer<QOpenGLFramebufferObject> fbo;
    QScopedPointer<GLWindowPaintDevice> paintDevice;
    QOpenGLTextureBlitter blitter;
    QColor backgroundColor;
    QScopedPointer<QOffscreenSurface> offscreenSurface;
};

GLWindowPrivate::~GLWindowPrivate()
{
    Q_Q(GLWindow);

    if (q->isValid())
    {
        q->makeCurrent(); // this works even when the platformwindow is destroyed

        paintDevice.reset(0);
        fbo.reset(0);
        blitter.destroy();

        q->doneCurrent();
    }
}

void GLWindowPrivate::initialize()
{
    Q_Q(GLWindow);

    if (context)
        return;

    if(shareContext)
        context.reset(shareContext);
    else
    {
        context.reset(new QOpenGLContext);
        context->setShareContext(shareContext);
        context->setFormat(q->requestedFormat());

        if (!context->create())
            qWarning("QOpenGLWindow::beginPaint: Failed to create context");
    }

    if (!context->makeCurrent(q))
        qWarning("QOpenGLWindow::beginPaint: Failed to make context current");

    paintDevice.reset(new GLWindowPaintDevice(q));

    if (updateBehavior == GLWindow::PartialUpdateBlit)
        hasFboBlit = QOpenGLFramebufferObject::hasOpenGLFramebufferBlit();

    q->initializeGL();
}

void GLWindowPrivate::beginPaint(const QRegion &region)
{
    Q_UNUSED(region);
    Q_Q(GLWindow);

    initialize();

    context->makeCurrent(q);

    const int deviceWidth = q->width() * q->devicePixelRatio();
    const int deviceHeight = q->height() * q->devicePixelRatio();

    if (updateBehavior > GLWindow::NoPartialUpdate)
    {
        const QSize deviceSize(deviceWidth, deviceHeight);

        if (!fbo || fbo->size() != deviceSize)
        {
            QOpenGLFramebufferObjectFormat fboFormat;
            fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

            const int samples = context->format().samples();

            if(samples > 0)
            {
                if (updateBehavior != GLWindow::PartialUpdateBlend)
                    fboFormat.setSamples(samples);
                else
                    qWarning("QOpenGLWindow: PartialUpdateBlend does not support multisampling");
            }

            fbo.reset(new QOpenGLFramebufferObject(deviceSize, fboFormat));

            markWindowAsDirty();
        }
    }
    else
        markWindowAsDirty();

    paintDevice->setSize(QSize(deviceWidth, deviceHeight));
    paintDevice->setDevicePixelRatio(q->devicePixelRatio());

    context->functions()->glViewport(0, 0, deviceWidth, deviceHeight);
    context->functions()->glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());

    q->paintUnderGL();

    if (updateBehavior > GLWindow::NoPartialUpdate)
        fbo->bind();
}

void GLWindowPrivate::endPaint()
{
    Q_Q(GLWindow);

    if (updateBehavior > GLWindow::NoPartialUpdate)
        fbo->release();

    // this sux -> keeps crashing
    //context->functions()->glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());

    if (updateBehavior == GLWindow::PartialUpdateBlit && hasFboBlit)
    {
        const int deviceWidth = q->width() * q->devicePixelRatio();
        const int deviceHeight = q->height() * q->devicePixelRatio();

        QOpenGLExtensions extensions(context.data());
        extensions.glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->handle());
        extensions.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, context->defaultFramebufferObject());
        extensions.glBlitFramebuffer(0, 0, deviceWidth, deviceHeight,
                                     0, 0, deviceWidth, deviceHeight,
                                     GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    else if (updateBehavior > GLWindow::NoPartialUpdate)
    {
        if (updateBehavior == GLWindow::PartialUpdateBlend)
        {
            context->functions()->glEnable(GL_BLEND);
            context->functions()->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        if (!blitter.isCreated())
            blitter.create();

        QRect windowRect(QPoint(0, 0), fbo->size());
        QMatrix4x4 target = QOpenGLTextureBlitter::targetTransform(windowRect, windowRect);

        blitter.bind();
        blitter.blit(fbo->texture(), target, QOpenGLTextureBlitter::OriginBottomLeft);
        blitter.release();

        if (updateBehavior == GLWindow::PartialUpdateBlend)
            context->functions()->glDisable(GL_BLEND);
    }

    q->paintOverGL();
}

void GLWindowPrivate::bindFBO()
{
    if (updateBehavior > GLWindow::NoPartialUpdate)
        fbo->bind();
    else
        QOpenGLFramebufferObject::bindDefault();
}

void GLWindowPrivate::flush(const QRegion& region)
{
    Q_UNUSED(region);
    Q_Q(GLWindow);

    //context->swapBuffers(q);

    emit q->frameSwapped();
}

/// Paint Device
void GLWindowPaintDevice::ensureActiveTarget()
{
    GLWindowPrivate::get(win)->bindFBO();
}

/// GLWindow2
GLWindow::GLWindow(UpdateBehavior updateBehavior, QWindow* parent)
: QPaintDeviceWindow(*(new GLWindowPrivate(Q_NULLPTR, updateBehavior)), parent)
{
    setSurfaceType(QSurface::OpenGLSurface);
}

GLWindow::GLWindow(QOpenGLContext* shareContext, UpdateBehavior updateBehavior, QWindow* parent)
: QPaintDeviceWindow(*(new GLWindowPrivate(shareContext, updateBehavior)), parent)
{
    setSurfaceType(QSurface::OpenGLSurface);
}

GLWindow::GLWindow(QOpenGLFunctions_4_5_Core* glFunctions, QOpenGLContext* shareContext, UpdateBehavior updateBehavior, QWindow* parent)
: QPaintDeviceWindow(*(new GLWindowPrivate(shareContext, updateBehavior)), parent)
, functions(glFunctions)
{
    setSurfaceType(QSurface::OpenGLSurface);
}

GLWindow::~GLWindow()
{
    makeCurrent();
}

bool GLWindow::isValid() const
{
    Q_D(const GLWindow);

    return d->context && d->context->isValid();
}

void GLWindow::makeCurrent()
{
    Q_D(GLWindow);

    if (!isValid())
        return;

    // The platform window may be destroyed at this stage and therefore
    // makeCurrent() may not safely be called with 'this'.
    if (handle())
        d->context->makeCurrent(this);
    else
    {
        if (!d->offscreenSurface)
        {
            d->offscreenSurface.reset(new QOffscreenSurface);
            d->offscreenSurface->setFormat(QSurfaceFormat::defaultFormat());
            d->offscreenSurface->create();
        }

        d->context->makeCurrent(d->offscreenSurface.data());
    }

    d->bindFBO();
}

void GLWindow::doneCurrent()
{
    Q_D(GLWindow);

    if (!isValid())
        return;

    d->context->doneCurrent();
}

QOpenGLContext* GLWindow::context() const
{
    Q_D(const GLWindow);

    return d->context.data();
}

GLuint GLWindow::defaultFramebufferObject() const
{
    Q_D(const GLWindow);

    if (d->updateBehavior > NoPartialUpdate && d->fbo)
        return d->fbo->handle();
    else if (QOpenGLContext* ctx = QOpenGLContext::currentContext())
        return ctx->defaultFramebufferObject();

    return 0;
}

extern Q_GUI_EXPORT QImage qt_gl_read_framebuffer(const QSize &size, bool alpha_format, bool include_alpha);

QImage GLWindow::grabFramebuffer()
{
    if (!isValid())
        return QImage();

    makeCurrent();

    return qt_gl_read_framebuffer(size() * devicePixelRatio(), false, false);
}


void GLWindow::initializeGL()
{
    if(!functions)
    {
        functions = context()->versionFunctions<QOpenGLFunctions_4_5_Core>();
        functions->initializeOpenGLFunctions();
    }

    initialize();

    renderNow();
}

void GLWindow::paintGL()
{
    functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    render();

    painted = true;

    if(fpsTimer.isActive())
        ++fps;
}

void GLWindow::paintUnderGL()
{
}

void GLWindow::paintOverGL()
{
}

void GLWindow::initialize()
{
}

void GLWindow::render()
{
}

void GLWindow::resizeGL(int width, int height)
{
    const qreal pixelRatio = devicePixelRatio();

    functions->glViewport(0, 0, width * pixelRatio, height * pixelRatio);
}

bool GLWindow::event(QEvent* event)
{
    switch(event->type())
    {
        case QEvent::UpdateRequest:
            {
                pendingUpdate = false;

                renderNow();
            }
            break;
    }

    return QPaintDeviceWindow::event(event);
}

void GLWindow::exposeEvent(QExposeEvent* event)
{
    Q_UNUSED(event);

    if(isExposed())
        renderNow();
}

void GLWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    paintGL();
}

void GLWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    Q_D(GLWindow);

    d->initialize();

    resizeGL(width(), height());
}

int GLWindow::metric(PaintDeviceMetric metric) const
{
    Q_D(const GLWindow);

    switch (metric)
    {
        case PdmDepth:
            if (d->paintDevice)
                return d->paintDevice->depth();
            break;

        default:
            break;
    }

    return QPaintDeviceWindow::metric(metric);
}

QPaintDevice* GLWindow::redirected(QPoint *) const
{
    Q_D(const GLWindow);

    if (QOpenGLContext::currentContext() == d->context.data())
        return d->paintDevice.data();

    return 0;
}

void GLWindow::renderNow()
{
    if(!isExposed())
        return;

    if(!painted)
        return;

    painted = false;

    makeCurrent();

    update();

    context()->swapBuffers(this);

    if(draw && !pendingUpdate)
    {
        pendingUpdate = false;

        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void GLWindow::setFpsCounter(bool enable)
{
    if(enable)
    {
        connect(&fpsTimer, &QTimer::timeout, this, &GLWindow::fpsTimeout);

        fpsTimer.start(1000);
    }
    else
    {
        disconnect(&fpsTimer, &QTimer::timeout, this, &GLWindow::fpsTimeout);

        fpsTimer.stop();
    }
}

void GLWindow::fpsTimeout()
{
    qDebug() << "FPS: " << fps;

    fps = 0;
}
