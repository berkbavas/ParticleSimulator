#include "FreeCamera.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QWheelEvent>
#include <QtMath>

const QMatrix4x4& ParticleSimulator::FreeCamera::GetProjectionMatrix()
{
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(mVerticalFov, GetAspectRatio(), GetZNear(), GetZFar());
    return mProjectionMatrix;
}

const QMatrix4x4& ParticleSimulator::FreeCamera::GetViewProjectionMatrix()
{
    mViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
    return mViewProjectionMatrix;
}

const QMatrix4x4& ParticleSimulator::FreeCamera::GetViewMatrix()
{
    mViewMatrix.setToIdentity();
    mViewMatrix.rotate(GetRotation().conjugated());
    mViewMatrix.translate(-GetPosition());
    return mViewMatrix;
}

const QMatrix4x4& ParticleSimulator::FreeCamera::GetRotationMatrix()
{
    constexpr QVector4D ZERO_TRANSLATION(0, 0, 0, 1);
    mRotationMatrix = GetViewMatrix();
    mRotationMatrix.setColumn(3, ZERO_TRANSLATION);
    return mRotationMatrix;
}

const QVector3D& ParticleSimulator::FreeCamera::GetViewDirection()
{
    mViewDirection = GetRotation() * NEGATIVE_Z;
    return mViewDirection;
}

void ParticleSimulator::FreeCamera::Resize(int width, int height)
{
    mWidth = width;
    mHeight = height;
}

float ParticleSimulator::FreeCamera::GetHorizontalFov() const
{
    const auto hfov = std::atan(std::atan(mVerticalFov / 2.0) / GetAspectRatio()) * 2.0f;

    return std::abs(qRadiansToDegrees(hfov));
}

float ParticleSimulator::FreeCamera::GetAspectRatio() const
{
    return float(mWidth) / float(mHeight);
}

void ParticleSimulator::FreeCamera::Update(float ifps)
{
    // Rotation
    if (mUpdateRotation)
    {
        RotateGlobal(QVector3D(0, 1, 0), mAngularSpeedMultiplier * mAngularSpeed * mMouse.dx * ifps);
        RotateLocal(QVector3D(1, 0, 0), mAngularSpeedMultiplier * mAngularSpeed * mMouse.dy * ifps);
        mMouse.dx = 0.0f;
        mMouse.dy = 0.0f;
        mUpdateRotation = false;
    }

    // Translation
    if (mUpdatePosition)
    {
        if (mPressedKeys[Qt::Key_Space])
        {
            mLinearSpeed = 50.0f;
        }
        else if (mPressedKeys[Qt::Key_Shift])
        {
            mLinearSpeed = 2.0f;
        }
        else if (mPressedKeys[Qt::Key_Control])
        {
            mLinearSpeed = 0.1f;
        }
        else
        {
            mLinearSpeed = 1.0f;
        }

        const auto keys = mPressedKeys.keys();

        for (const auto key : keys)
        {
            if (mPressedKeys.value(key, false))
            {
                const auto binding = KEY_BINDINGS.value(key, QVector3D(0, 0, 0));
                const auto direction = GetRotation().rotatedVector(binding);
                const auto delta = mLinearSpeed * mLinearSpeedMultiplier * ifps * direction;
                Translate(delta);
            }
        }
    }

    if (mPressedKeys.empty())
    {
        mUpdatePosition = false;
    }
}

void ParticleSimulator::FreeCamera::Reset()
{
    const auto keys = mPressedKeys.keys();

    for (const auto key : keys)
    {
        mPressedKeys.insert(key, false);
    }

    mUpdatePosition = false;
    mUpdateRotation = false;
    mMouse.Reset();
}

void ParticleSimulator::FreeCamera::OnKeyPressed(QKeyEvent* event)
{
    mPressedKeys.insert((Qt::Key) event->key(), true);
    mUpdatePosition = true;
}

void ParticleSimulator::FreeCamera::OnKeyReleased(QKeyEvent* event)
{
    mPressedKeys.insert((Qt::Key) event->key(), false);
}

void ParticleSimulator::FreeCamera::OnMousePressed(QMouseEvent* event)
{
    mMouse.x = event->position().x();
    mMouse.y = event->position().y();
    mMouse.button = event->button();
}

void ParticleSimulator::FreeCamera::OnMouseReleased(QMouseEvent* event)
{
    if (mMouse.button == event->button())
    {
        mMouse.button = Qt::NoButton;
    }
}

void ParticleSimulator::FreeCamera::OnMouseMoved(QMouseEvent* event)
{
    if (mMouse.button == mActionReceiveButton)
    {
        mMouse.dx += mMouse.x - event->position().x();
        mMouse.dy += mMouse.y - event->position().y();

        mMouse.x = event->position().x();
        mMouse.y = event->position().y();
        mUpdateRotation = true;
    }
}

const QMap<Qt::Key, QVector3D> ParticleSimulator::FreeCamera::KEY_BINDINGS = //
    {
        { Qt::Key_W, QVector3D(0, 0, -1) }, //
        { Qt::Key_S, QVector3D(0, 0, 1) },  //
        { Qt::Key_A, QVector3D(-1, 0, 0) }, //
        { Qt::Key_D, QVector3D(1, 0, 0) },  //
        { Qt::Key_E, QVector3D(0, 1, 0) },  //
        { Qt::Key_Q, QVector3D(0, -1, 0) }  //
    };
