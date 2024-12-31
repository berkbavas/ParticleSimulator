#pragma once

#include "Node/FreeCamera.h"
#include "Renderer/Shader.h"
#include "Renderer/Window.h"
#include "Simulation/ParticleSimulation.h"

#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFunctions_4_3_Core>

namespace ParticleSimulator
{
    class Renderer : public QObject, protected QOpenGLFunctions_4_3_Core
    {
      public:
        Renderer(QObject* parent = nullptr);
        ~Renderer();

        void Run();

        void Initialize();
        void Resize(int w, int h);
        void Render(float ifps);

        void OnKeyPressed(QKeyEvent*);
        void OnKeyReleased(QKeyEvent*);
        void OnMousePressed(QMouseEvent*);
        void OnMouseReleased(QMouseEvent*);
        void OnMouseMoved(QMouseEvent*);
        void OnWheelMoved(QWheelEvent*);

      private:
        float mDevicePixelRatio{ 1.0f };
        float mWidth{ 1600 };
        float mHeight{ 900 };

        Window* mWindow;
        FreeCamera* mCamera;

        ParticleSimulation* mParticleSimulation;
        Shader* mPointCloudShader;

        float mSpeed{ 1.0f };

        float mDiffuseFactor{ 0.75f };
        float mAmbientFactor{ 0.25f };
        float mColorMultiplier{ 0.25f };
    };
}