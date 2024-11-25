#pragma once

#include "Node/FreeCamera.h"
#include "Simulation/ParticleSimulation.h"
#include "Renderer/Shader.h"
#include "Renderer/Window.h"

#include <QMouseEvent>
#include <QObject>
#include <QOpenGLExtraFunctions>

namespace ParticleSimulator
{
    class Renderer : public QObject, protected QOpenGLExtraFunctions
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
    };
}