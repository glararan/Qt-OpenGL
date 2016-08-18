#ifndef CUBE_H
#define CUBE_Háí

#include "GLObject.h"

class Cube : public GLObject
{
    Q_OBJECT

public:
    explicit Cube(QObject* parent = 0);
    ~Cube();

    void Initialize(QOpenGLFunctions_4_5_Core* glFunctions) Q_DECL_OVERRIDE;
    void Draw(const QMatrix4x4& mvp) Q_DECL_OVERRIDE;
    void Draw2(const QMatrix4x4& view, const QMatrix4x4& proj);

private:
    int modelLoc;
    int viewLoc;
    int projectionLoc;
};

#endif // CUBE_H
