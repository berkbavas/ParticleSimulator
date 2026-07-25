#pragma once

#include <QInputEvent>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

namespace ParticleSimulator
{
    class Window : public QOpenGLWidget
    {
        Q_OBJECT
      public:
        explicit Window(QWidget* pParent = nullptr);

      private:
        void initializeGL() override;
        void resizeGL(int Width, int Height) override;
        void paintGL() override;
        void keyPressEvent(QKeyEvent* pEvent) override;
        void keyReleaseEvent(QKeyEvent* pEvent) override;
        void mousePressEvent(QMouseEvent* pEvent) override;
        void mouseReleaseEvent(QMouseEvent* pEvent) override;
        void mouseMoveEvent(QMouseEvent* pEvent) override;
        void wheelEvent(QWheelEvent* pEvent) override;
        void leaveEvent(QEvent* pEvent) override;
        void focusOutEvent(QFocusEvent* pEvent) override;

      signals:
        // Core Events
        void Initialize();
        void Resize(int Width, int Height);
        void Render(float Ifps);

        // Input Events
        void KeyPressed(QKeyEvent* pEvent);
        void KeyReleased(QKeyEvent* pEvent);
        void MousePressed(QMouseEvent* pEvent);
        void MouseReleased(QMouseEvent* pEvent);
        void MouseMoved(QMouseEvent* pEvent);
        void WheelMoved(QWheelEvent* pEvent);
        void LeaveEvent(QEvent* pEvent);
        void FocusOutEvent(QFocusEvent* pEvent);

      private:
        long long mPreviousTime;
        long long mCurrentTime;
    };
}
