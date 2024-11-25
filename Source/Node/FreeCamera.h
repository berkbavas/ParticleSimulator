#pragma once

#include "Node/Mouse.h"
#include "Node/Node.h"

#include <QMap>
#include <QMouseEvent>

namespace ParticleSimulator
{
    class FreeCamera : public Node
    {
      public:
        FreeCamera() = default;

        void Resize(int w, int h);
        void Update(float ifps);
        void Reset();

        const QMatrix4x4 &GetProjectionMatrix();
        const QMatrix4x4 &GetViewProjectionMatrix();
        const QMatrix4x4 &GetRotationMatrix();
        const QMatrix4x4 &GetViewMatrix();
        const QVector3D &GetViewDirection();

        void OnKeyPressed(QKeyEvent *);
        void OnKeyReleased(QKeyEvent *);
        void OnMousePressed(QMouseEvent *);
        void OnMouseReleased(QMouseEvent *);
        void OnMouseMoved(QMouseEvent *);

        float GetHorizontalFov() const;
        float GetAspectRatio() const;

      private:
        // For caching
        QMatrix4x4 mViewProjectionMatrix;
        QMatrix4x4 mProjectionMatrix;
        QMatrix4x4 mViewMatrix;
        QMatrix4x4 mRotationMatrix;
        QVector3D mViewDirection;

        DEFINE_MEMBER(int, Width, 1600);
        DEFINE_MEMBER(int, Height, 900);

        DEFINE_MEMBER(float, ZNear, 0.01f);
        DEFINE_MEMBER(float, ZFar, 100000.0f);
        DEFINE_MEMBER(float, VerticalFov, 60.0f);

        DEFINE_MEMBER(float, LinearSpeed, 1.0f);
        DEFINE_MEMBER(float, LinearSpeedMultiplier, 1.0f);
        DEFINE_MEMBER(float, AngularSpeed, 20.0f);
        DEFINE_MEMBER(float, AngularSpeedMultiplier, 1.0f);
        DEFINE_MEMBER(Qt::MouseButton, ActionReceiveButton, Qt::MiddleButton)

        QMap<Qt::Key, bool> mPressedKeys;

        Mouse mMouse;

        bool mUpdateRotation{ false };
        bool mUpdatePosition{ false };

        static const QMap<Qt::Key, QVector3D> KEY_BINDINGS;

        static constexpr QVector3D POSITIVE_Z = QVector3D(0, 0, 1);
        static constexpr QVector3D NEGATIVE_Z = QVector3D(0, 0, -1);
    };

    using FreeCameraPtr = std::shared_ptr<FreeCamera>;

}
