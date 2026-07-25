#include "Window.h"

#include "Util/Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>

ParticleSimulator::Window::Window(QWidget* pParent)
    : QOpenGLWidget(pParent)
{
    connect(this, &QOpenGLWidget::frameSwapped, [=]() { update(); });
}

void ParticleSimulator::Window::initializeGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;

    emit Initialize();
}

void ParticleSimulator::Window::resizeGL(int Width, int Height)
{
    emit Resize(Width, Height);
}

void ParticleSimulator::Window::paintGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    const float Ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    emit Render(Ifps);
}

void ParticleSimulator::Window::keyPressEvent(QKeyEvent* pEvent)
{
    emit KeyPressed(pEvent);
}

void ParticleSimulator::Window::keyReleaseEvent(QKeyEvent* pEvent)
{
    emit KeyReleased(pEvent);
}

void ParticleSimulator::Window::mousePressEvent(QMouseEvent* pEvent)
{
    emit MousePressed(pEvent);
}

void ParticleSimulator::Window::mouseReleaseEvent(QMouseEvent* pEvent)
{
    emit MouseReleased(pEvent);
}

void ParticleSimulator::Window::mouseMoveEvent(QMouseEvent* pEvent)
{
    emit MouseMoved(pEvent);
}

void ParticleSimulator::Window::wheelEvent(QWheelEvent* pEvent)
{
    emit WheelMoved(pEvent);
}

void ParticleSimulator::Window::leaveEvent(QEvent* pEvent)
{
    emit LeaveEvent(pEvent);
}

void ParticleSimulator::Window::focusOutEvent(QFocusEvent* pEvent)
{
    emit FocusOutEvent(pEvent);
}
