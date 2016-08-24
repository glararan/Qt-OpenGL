#ifndef GLMANAGER_H
#define GLMANAGER_H

#include <QObject>

#include <QOpenGLContext>

class GLManager : public QObject
{
    Q_OBJECT

public:
    explicit GLManager(QObject* parent = 0);
    ~GLManager();

    QOpenGLContext* context() const { return glContext; }

private:
    QOpenGLContext* glContext = Q_NULLPTR;

signals:

public slots:
};

#endif // GLMANAGER_H
