#include "Renderer.h"

#include "Util/Logger.h"

#include <imgui.h>

#include <QThread>
#include <QtImGui.h>

ParticleSimulator::Renderer::Renderer(QObject *parent)
    : QObject(parent)
{
    qInstallMessageHandler(ParticleSimulator::Logger::QtMessageOutputCallback);

    mWindow = new Window;
    mCamera = new FreeCamera;

    connect(mWindow, &Window::Initialize, this, &Renderer::Initialize);
    connect(mWindow, &Window::Render, this, &Renderer::Render);
    connect(mWindow, &Window::Resize, this, &Renderer::Resize);
    connect(mWindow, &Window::MousePressed, this, &Renderer::OnMousePressed);
    connect(mWindow, &Window::MouseReleased, this, &Renderer::OnMouseReleased);
    connect(mWindow, &Window::MouseMoved, this, &Renderer::OnMouseMoved);
    connect(mWindow, &Window::WheelMoved, this, &Renderer::OnWheelMoved);
    connect(mWindow, &Window::KeyPressed, this, &Renderer::OnKeyPressed);
    connect(mWindow, &Window::KeyReleased, this, &Renderer::OnKeyReleased);
}

ParticleSimulator::Renderer::~Renderer()
{
    qDebug() << "Renderer::~Renderer: Application closing...";
    qDebug() << "Renderer::~Renderer: Current Thread:" << QThread::currentThread();
}

void ParticleSimulator::Renderer::Run()
{
    qDebug() << "Renderer::Run: Application starting...";

    mWindow->resize(mWidth, mHeight);
    mWindow->show();
}

void ParticleSimulator::Renderer::Initialize()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    mParticleSimulation = new ParticleSimulation(500'000, 8);

    mPointCloudShader = new Shader("Point Cloud Shader");
    mPointCloudShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/PointCloud.vert");
    mPointCloudShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/PointCloud.frag");
    mPointCloudShader->Initialize();

    QtImGui::initialize(mWindow, true);
}

void ParticleSimulator::Renderer::Resize(int width, int height)
{
    mDevicePixelRatio = mWindow->devicePixelRatio();
    mWidth = mDevicePixelRatio * width;
    mHeight = mDevicePixelRatio * height;

    mCamera->Resize(mWidth, mHeight);
}

void ParticleSimulator::Renderer::Render(float ifps)
{
    mCamera->Update(ifps);

    mParticleSimulation->Update(ifps);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mPointCloudShader->Bind();
    mPointCloudShader->SetUniformValue("VP", mCamera->GetViewProjectionMatrix());

    glBindVertexArray(mParticleSimulation->GetVertexArrayObject());
    glDrawArrays(GL_POINTS, 0, mParticleSimulation->GetNumberOfParticles());

    mPointCloudShader->Release();

    QtImGui::newFrame();

    ImGui::Begin("Debug");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    QtImGui::render();
}

void ParticleSimulator::Renderer::OnKeyPressed(QKeyEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
    {
        return;
    }

    mCamera->OnKeyPressed(pEvent);
}

void ParticleSimulator::Renderer::OnKeyReleased(QKeyEvent *pEvent)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
    {
        return;
    }

    mCamera->OnKeyReleased(pEvent);
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
}
