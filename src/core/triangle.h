#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "globject.h"

class Triangle : public GLObject
{
    Q_OBJECT

public:
    explicit Triangle(QObject* parent = 0);
    ~Triangle();

    void Initialize(QOpenGLFunctions_4_5_Core* glFunctions) Q_DECL_OVERRIDE;
    void Draw(const QMatrix4x4& mvp) Q_DECL_OVERRIDE;

private:
    int mvpLoc;
};

#endif // TRIANGLE_H
