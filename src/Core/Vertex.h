#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

class Vertex
{
public:
    Q_DECL_CONSTEXPR Vertex()
    {
    }

    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D& position)
    : vPosition(position)
    {
    }

    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D& position, const QVector3D& color)
    : vPosition(position)
    , vColor(color)
    {
    }

    Q_DECL_CONSTEXPR const QVector3D& position() const { return vPosition; }
    Q_DECL_CONSTEXPR const QVector3D& color() const    { return vColor; }

    void setPosition(const QVector3D& position) { vPosition = position; }
    void setColor(const QVector3D& color)       { vColor = color; }

    static const int PositionSize = 3;
    static const int ColorSize    = 3;

    static Q_DECL_CONSTEXPR int positionOffset() { return offsetof(Vertex, vPosition); }
    static Q_DECL_CONSTEXPR int colorOffset()    { return offsetof(Vertex, vColor); }
    static Q_DECL_CONSTEXPR int stride()         { return sizeof(Vertex); }

private:
    QVector3D vPosition;
    QVector3D vColor;
};

Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

#endif // VERTEX_H
