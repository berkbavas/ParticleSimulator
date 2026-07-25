#include "Renderer.h"

#include "Util/Logger.h"

#include <imgui.h>

#include <QOpenGLFramebufferObject>
#include <QThread>
#include <QtImGui.h>

ParticleSimulator::Renderer::Renderer(QObject *pParent)
    : QObject(pParent)
{
    mWindow = new Window;
    mCamera = new PersecutorCamera;

    connect(mWindow, &Window::Initialize, this, &Renderer::Initialize);
    connect(mWindow, &Window::Render, this, &Renderer::Render);
    connect(mWindow, &Window::Resize, this, &Renderer::Resize);
    connect(mWindow, &Window::MousePressed, this, &Renderer::OnMousePressed);
    connect(mWindow, &Window::MouseReleased, this, &Renderer::OnMouseReleased);
    connect(mWindow, &Window::MouseMoved, this, &Renderer::OnMouseMoved);
    connect(mWindow, &Window::WheelMoved, this, &Renderer::OnWheelMoved);
}

ParticleSimulator::Renderer::~Renderer()
{
    qDebug() << "Renderer::~Renderer: Application closing...";
    qDebug() << "Renderer::~Renderer: Current Thread:" << QThread::currentThread();
}

void ParticleSimulator::Renderer::Run()
{
    qDebug() << "Renderer::Run: Application starting...";

    qInstallMessageHandler(ParticleSimulator::Logger::QtMessageOutputCallback);

    mWindow->showMinimized();
}

void ParticleSimulator::Renderer::Initialize()
{
    initializeOpenGLFunctions();

    glDisable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    mParticleSimulation = new ParticleSimulation;

    mPointCloudShader = new Shader("Point Cloud Shader");
    mPointCloudShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/PointCloud.vert");
    mPointCloudShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/PointCloud.frag");
    mPointCloudShader->Initialize();

    QtImGui::initialize(mWindow, true);

    mWindow->showMaximized();
}

void ParticleSimulator::Renderer::Resize(int Width, int Height)
{
    mDevicePixelRatio = mWindow->devicePixelRatio();
    mWidth = mDevicePixelRatio * Width;
    mHeight = mDevicePixelRatio * Height;

    mCamera->Resize(mWidth, mHeight);
}

void ParticleSimulator::Renderer::Render(float Ifps)
{
    mCamera->Update(Ifps);

    mTime += Ifps * mSpeed;

    mParticleSimulation->Update(Ifps * mSpeed);

    QOpenGLFramebufferObject::bindDefault();
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(mClearColor.x(), mClearColor.y(), mClearColor.z(), 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    if (mEnableBlending)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(GL_FALSE);
    }

    mPointCloudShader->Bind();
    mPointCloudShader->SetUniform("uVP", mCamera->GetViewProjectionMatrix());
    mPointCloudShader->SetUniform("uAmbient", mAmbientFactor);
    mPointCloudShader->SetUniform("uDiffuse", mDiffuseFactor);
    mPointCloudShader->SetUniform("uColorMultiplier", mColorMultiplier);
    mPointCloudShader->SetUniform("uLightDirection", mCamera->GetViewDirection());
    mPointCloudShader->SetUniform("uBasePointSize", mBasePointSize);
    mPointCloudShader->SetUniform("uTime", mTime);

    glBindVertexArray(mParticleSimulation->GetVertexArrayObject());
    glDrawArrays(GL_POINTS, 0, mParticleSimulation->GetNumberOfParticles());

    mPointCloudShader->Unbind();

    if (mEnableBlending)
    {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }

    QtImGui::newFrame();

    ImGui::Begin("Particle Simulation Control");

    ImGui::Text("FPS: %.1f (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text("Particles: %zu", mParticleSimulation->GetNumberOfParticles());
    ImGui::Separator();

    ImGui::Text("Simulation Mode");
    const char *Modes[] = { "Attraction", "Vortex", "Explosion", "Galaxy", "Fluid", "Fireworks", "Swarm", "Black Hole" };
    if (ImGui::Combo("Mode", &mCurrentSimMode, Modes, IM_ARRAYSIZE(Modes)))
    {
        mParticleSimulation->SetSimulationMode(static_cast<SimulationMode>(mCurrentSimMode));
    }

    if (ImGui::Button("Reset Simulation"))
    {
        mParticleSimulation->Reset();
        mTime = 0.0f;
    }

    ImGui::Separator();

    ImGui::Text("Simulation");
    ImGui::SliderFloat("Speed", &mSpeed, 0.001f, 10.0f);
    ImGui::Checkbox("Apply Forces", &mParticleSimulation->GetApplyAttractionForce_NonConst());

    ImGui::Separator();

    ImGui::Text("Physics");
    ImGui::SliderFloat("Gravity##Slider", &mParticleSimulation->GetGravity_NonConst(), -5.0f, 5.0f);
    ImGui::SliderFloat("Damping##Slider", &mParticleSimulation->GetDamping_NonConst(), 0.9f, 1.0f);
    ImGui::SliderFloat("Turbulence##Slider", &mParticleSimulation->GetTurbulenceStrength_NonConst(), 0.0f, 2.0f);
    ImGui::SliderFloat("Vortex##Slider", &mParticleSimulation->GetVortexStrength_NonConst(), 0.0f, 10.0f);
    ImGui::SliderFloat("Attraction##Slider", &mParticleSimulation->GetAttractionStrength_NonConst(), 0.0f, 5.0f);
    ImGui::SliderFloat("Repulsion##Slider", &mParticleSimulation->GetRepulsionStrength_NonConst(), 0.0f, 2.0f);
    ImGui::SliderFloat("Boundary##Slider", &mParticleSimulation->GetBoundarySize_NonConst(), 1.0f, 20.0f);

    ImGui::Separator();

    ImGui::Text("Features");
    ImGui::Checkbox("Gravity##Checkbox", &mParticleSimulation->GetEnableGravity_NonConst());
    ImGui::SameLine();
    ImGui::Checkbox("Turbulence##Checkbox", &mParticleSimulation->GetEnableTurbulence_NonConst());
    ImGui::Checkbox("Vortex##Checkbox", &mParticleSimulation->GetEnableVortex_NonConst());
    ImGui::SameLine();
    ImGui::Checkbox("Boundary##Checkbox", &mParticleSimulation->GetEnableBoundary_NonConst());

    ImGui::Separator();

    ImGui::Text("Visuals");
    ImGui::SliderFloat("Point Size##Slider", &mBasePointSize, 1.0f, 10.0f);
    ImGui::SliderFloat("Ambient##Slider", &mAmbientFactor, 0.0f, 1.0f);
    ImGui::SliderFloat("Diffuse##Slider", &mDiffuseFactor, 0.0f, 1.0f);
    ImGui::SliderFloat("Color Mult##Slider", &mColorMultiplier, 0.0f, 1.0f);
    ImGui::Checkbox("Glow Effect##Checkbox", &mEnableBlending);
    ImGui::End();

    ImGui::Render();
    QtImGui::render();
}

void ParticleSimulator::Renderer::OnMousePressed(QMouseEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mCamera->OnMousePressed(pEvent);
}

void ParticleSimulator::Renderer::OnMouseReleased(QMouseEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mCamera->OnMouseReleased(pEvent);
}

void ParticleSimulator::Renderer::OnMouseMoved(QMouseEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mCamera->OnMouseMoved(pEvent);
}

void ParticleSimulator::Renderer::OnWheelMoved(QWheelEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mCamera->OnWheelMoved(pEvent);
}
