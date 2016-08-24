#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>
#include <QVector4D>

class Camera : public QObject
{
    Q_OBJECT

public:
    enum Translation
    {
        XYZ,
        XZ
    };

    explicit Camera(QObject* parent = 0);
    explicit Camera(const QVector3D& pos, QObject* parent = 0);
    ~Camera();

    /*const QVector3D& getDirection() const { return (center - position).normalized(); }
    const QVector3D& getRight() const     { return QVector3D::crossProduct(up, getDirection()).normalized(); }
    const QVector3D& getUp() const        { return QVector3D::crossProduct(getDirection(), getRight()); }*/

    const QVector3D& getRight() const     { return QVector3D::crossProduct(center, up).normalized(); }
    const QVector3D& getUp() const        { return QVector3D::crossProduct(getRight(), center).normalized(); }
    const QVector3D& getRight2() const    { return rotation.rotatedVector(QVector3D(1.0f, 0.0f, 0.0f)); }

    const QVector3D& getCenter() const { return center; }

    const float getPitch() const { return pitch; }
    const float getYaw() const   { return yaw; }
    const float getSpeed() const { return speed; }

    const QMatrix4x4& getView();
    const QMatrix4x4& getProjection();
    const QMatrix4x4& getViewProjection();

    void lookAtWithRadius(const float& delta, const float radius = 10.0f);

    void translate(const QVector3D& translation, const Translation type = XYZ);
    void rotate(const float& angle, const QVector3D& vector);

    void processKey(const int& key, const double& deltaTime);
    void updateCenter(const QVector2D& value);

    void setPerspective(const float angle, const float aspectRatio, const float nearPlane, const float farPlane);

    void setPitch(const float& value) { pitch = qBound(-89.0f, value, 89.0f); }
    void setYaq(const float& value)   { yaw = value; }
    void setSpeed(const float& value) { speed = value; }

private:
    QVector3D position = QVector3D(0.0f, 0.0f, 3.0f);
    QVector3D center   = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D up       = QVector3D(0.0f, 1.0f, 0.0f);

    QMatrix4x4 view;
    QMatrix4x4 projection;

    QQuaternion rotation = QQuaternion::fromEulerAngles(0.0f, -90.0f, 0.0f);

    float pitch = 0.0f;
    float yaw   = -90.0f;

    float speed = 5.0f;

    bool changed = false;

signals:

public slots:
};

#endif // CAMERA_H
