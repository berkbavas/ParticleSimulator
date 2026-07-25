#include "Mouse.h"

void ParticleSimulator::Mouse::SetButtonPressed(Qt::MouseButton Button, bool Pressed)
{
    mButtonPressedStates[Button] = Pressed;
}

bool ParticleSimulator::Mouse::IsButtonPressed(Qt::MouseButton Button) const
{
    return mButtonPressedStates.value(Button, false);
}

void ParticleSimulator::Mouse::SetLastPressPosition(Qt::MouseButton Button, const QPointF &Position)
{
    mButtonLastPressPositions[Button] = Position;
}

QPointF ParticleSimulator::Mouse::GetLastPressPosition(Qt::MouseButton Button) const
{
    return mButtonLastPressPositions.value(Button, QPointF());
}

void ParticleSimulator::Mouse::AddCumulativeMovement(Qt::MouseButton Button, const QPointF &Movement)
{
    mButtonCumulativeMovements[Button] += Movement;
}

void ParticleSimulator::Mouse::ConsumeCumulativeMovement(Qt::MouseButton Button, const QPointF &Movement)
{
    mButtonCumulativeMovements[Button] -= Movement;
}

QPointF ParticleSimulator::Mouse::GetCumulativeMovement(Qt::MouseButton Button) const
{
    return mButtonCumulativeMovements.value(Button, QPointF());
}

void ParticleSimulator::Mouse::ResetCumulativeMovementIfAlmostZero(Qt::MouseButton Button)
{
    const auto Movement = mButtonCumulativeMovements.value(Button, QPointF());

    if (std::abs(Movement.x()) < 0.1f)
    {
        mButtonCumulativeMovements[Button] = QPointF(0, Movement.y());
    }

    if (std::abs(Movement.y()) < 0.1f)
    {
        mButtonCumulativeMovements[Button] = QPointF(Movement.x(), 0);
    }
}

void ParticleSimulator::Mouse::ResetCumulativeMovement(Qt::MouseButton Button)
{
    mButtonCumulativeMovements[Button] = QPointF();
}

void ParticleSimulator::Mouse::Reset()
{
    mButtonPressedStates.clear();
    mButtonLastPressPositions.clear();
    mButtonCumulativeMovements.clear();
}
