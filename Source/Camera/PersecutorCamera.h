#pragma once

#include "Camera/PerspectiveCamera.h"
#include "Util/Mouse.h"

namespace ParticleSimulator
{
    class PersecutorCamera final : public PerspectiveCamera
    {
      public:
        PersecutorCamera() = default;

        void Update(float Ifps) override;
        void Reset() override;

        bool OnMousePressed(QMouseEvent *pEvent);
        bool OnMouseReleased(QMouseEvent *pEvent);
        bool OnMouseMoved(QMouseEvent *pEvent);
        bool OnWheelMoved(QWheelEvent *pEvent);

        Node *GetTarget() const;
        void SetTarget(Node *pNewTarget);

      private:
        void ClampAngles();
        void HandleZoom(float Ifps);
        void HandleRotation(float Ifps);
        void HandleTranslation(float Ifps);

        // Internal state
        Node *mTarget{ nullptr };
        Mouse mMouse;
        float mDistanceBuffer{ 0.0f };
        QVector3D mTranslation;

        float mYaw{ 0.0f };
        float mPitch{ 0.0f };
        float mDistance{ 5.0f };

        // Traits
        DEFINE_MEMBER(float, AngularSpeed, 12.5f);
        DEFINE_MEMBER(float, AngularSpeedSmoothness, 0.25f);
        DEFINE_MEMBER(float, LinearSpeed, 1.0f);
        DEFINE_MEMBER(float, LinearSpeedSmoothness, 0.25f);
        DEFINE_MEMBER(float, ZoomStep, 0.25f);
        DEFINE_MEMBER(float, ZoomSmoothness, 10.0f);

        static constexpr float MIN_DISTANCE{ 0.1f };
        static constexpr float MAX_DISTANCE{ 25.0f };

        static constexpr QVector3D POSITIVE_X{ 1, 0, 0 };
        static constexpr QVector3D NEGATIVE_Y{ 0, -1, 0 };
        static constexpr QVector3D POSITIVE_Y{ 0, 1, 0 };
        static constexpr QVector3D POSITIVE_Z{ 0, 0, 1 };
        static constexpr QVector3D NEGATIVE_Z{ 0, 0, -1 };
    };

    using PersecutorCameraPtr = std::unique_ptr<PersecutorCamera>;
}
