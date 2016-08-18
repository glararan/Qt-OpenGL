#include "Camera.h"

#include <QTime>
#include <QTimer>
#include <QtMath>
#include <QVector2D>

Camera::Camera(QObject* parent)
: QObject(parent)
{
}

Camera::Camera(const QVector3D& pos, QObject* parent)
: QObject(parent)
, position(pos)
{
}

Camera::~Camera()
{
}

const QMatrix4x4& Camera::getView()
{
    if(changed)
    {
        view.setToIdentity();
        view.lookAt(position, position + center, up);

        changed = false;
    }

    return view;
}

const QMatrix4x4& Camera::getProjection()
{
    return projection;
}

const QMatrix4x4& Camera::getViewProjection()
{
    return getView() * getProjection();
}

void Camera::lookAtWithRadius(const float& delta, const float radius)
{
    float x = qSin(delta) * radius;
    float z = qCos(delta) * radius;

    QVector3D eye = QVector3D(x, 0.0f, z);// + position;

    /*view.setToIdentity();
    view.lookAt(eye, eye + center, up);*/

    position = eye;

    changed = true;
}

void Camera::setPerspective(const float angle, const float aspectRatio, const float nearPlane, const float farPlane)
{
    projection.setToIdentity();
    projection.perspective(angle, aspectRatio, nearPlane, farPlane);

    view.setToIdentity();
    view.lookAt(position, position + center, up);
}

void Camera::translate(const QVector3D& translation, const Translation type)
{
    if(type == XYZ)
        position += translation;
    else
        position += QVector3D(translation.x(), 0.0f, translation.z());

    changed = true;
}

void Camera::rotate(const float& angle, const QVector3D& vector)
{
    rotation *= QQuaternion::fromAxisAndAngle(vector, angle);

    changed = true;
}

void Camera::processKey(const int& key, const double& deltaTime)
{
    float velocity = speed * deltaTime;

    QVector3D right = QVector3D(qSin(yaw - 3.14f / 2.0f), 0.0f, qCos(yaw - 3.14f / 2.0f));

    switch(key)
    {
        case Qt::Key_W:
            translate(velocity * getCenter());
            break;

        case Qt::Key_S:
            translate(velocity * -getCenter());
            break;

        case Qt::Key_A:
            translate(velocity * right, XZ);//translate(speed * -getRight().normalized(), Camera::XZ);
            break;

        case Qt::Key_D:
            translate(velocity * -right, XZ);//translate(speed * getRight().normalized(), Camera::XZ);
            break;

        case Qt::Key_Space:
            translate(QVector3D(0.0f, 0.1f, 0.0f));
            break;

        case Qt::Key_X:
            translate(QVector3D(0.0f, -0.1f, 0.0f));
            break;
    }
}

void Camera::updateCenter(const QVector2D& value)
{
    pitch = qBound(-89.0f, pitch + value.y(), 89.0f);
    yaw  -= value.x();

    QVector3D direction;
    direction.setX(qCos(qDegreesToRadians(pitch)) * qCos(qDegreesToRadians(yaw)));
    direction.setZ(qCos(qDegreesToRadians(pitch)) * qSin(qDegreesToRadians(yaw)));
    direction.setY(qSin(qDegreesToRadians(pitch)));

    center = direction.normalized();

    changed = true;
}
