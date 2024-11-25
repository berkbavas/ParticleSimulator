#include "Window.h"

#include "Util/Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>

ParticleSimulator::Window::Window(QWindow* parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)
{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(4);
    setFormat(format);

    connect(this, &QOpenGLWindow::frameSwapped, [=]() { update(); });
}

void ParticleSimulator::Window::initializeGL()
{
    initializeOpenGLFunctions();

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;

    emit Initialize();
}

void ParticleSimulator::Window::resizeGL(int width, int height)
{
    emit Resize(width, height);
}

void ParticleSimulator::Window::paintGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    const float ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    emit Render(ifps);
}

void ParticleSimulator::Window::keyPressEvent(QKeyEvent* event)
{
    emit KeyPressed(event);
}

void ParticleSimulator::Window::keyReleaseEvent(QKeyEvent* event)
{
    emit KeyReleased(event);
}

void ParticleSimulator::Window::mousePressEvent(QMouseEvent* event)
{
    emit MousePressed(event);
}

void ParticleSimulator::Window::mouseReleaseEvent(QMouseEvent* event)
{
    emit MouseReleased(event);
}

void ParticleSimulator::Window::mouseMoveEvent(QMouseEvent* event)
{
    emit MouseMoved(event);
}

void ParticleSimulator::Window::wheelEvent(QWheelEvent* event)
{
    emit WheelMoved(event);
}
