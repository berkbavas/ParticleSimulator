#pragma once

#include "Camera/PersecutorCamera.h"
#include "Renderer/Shader.h"
#include "Renderer/Window.h"
#include "Simulation/ParticleSimulation.h"

#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFunctions_4_5_Core>

namespace ParticleSimulator
{
    class Renderer : public QObject, protected QOpenGLFunctions_4_5_Core
    {
      public:
        explicit Renderer(QObject* pParent = nullptr);
        ~Renderer();

        void Run();

        void Initialize();
        void Resize(int Width, int Height);
        void Render(float Ifps);
        void OnMousePressed(QMouseEvent* pEvent);
        void OnMouseReleased(QMouseEvent* pEvent);
        void OnMouseMoved(QMouseEvent* pEvent);
        void OnWheelMoved(QWheelEvent* pEvent);

      private:
        float mDevicePixelRatio{ 1.0f };
        int mWidth{ 1600 };
        int mHeight{ 900 };

        Window* mWindow;
        PersecutorCamera* mCamera;
        ParticleSimulation* mParticleSimulation;
        Shader* mPointCloudShader;

        float mSpeed{ 1.0f };
        float mTime{ 0.0f };

        float mDiffuseFactor{ 0.75f };
        float mAmbientFactor{ 0.25f };
        float mColorMultiplier{ 0.25f };
        float mBasePointSize{ 3.0f };

        int mCurrentSimMode{ 0 };
        bool mEnableBlending{ true };

        QVector3D mClearColor{ 0.02f, 0.02f, 0.05f };
    };
}