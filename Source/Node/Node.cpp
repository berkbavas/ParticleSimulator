#include "Node.h"

void ParticleSimulator::Node::SetRotation(const QQuaternion& newRotation)
{
    mRotation = newRotation;
    MakeTransformationDirty();
}

void ParticleSimulator::Node::SetPosition(const QVector3D& newPosition)
{
    mPosition = newPosition;
    MakeTransformationDirty();
}

void ParticleSimulator::Node::SetScale(const QVector3D& newScale)
{
    mScale = newScale;
    MakeTransformationDirty();
}

const QMatrix4x4& ParticleSimulator::Node::GetTransformation()
{
    if (mTransformationDirty)
    {
        UpdateTransformation();
    }

    return mTransformation;
}

void ParticleSimulator::Node::SetTransformation(const QMatrix4x4& newTransformation)
{
    mTransformation = newTransformation;
    mPosition = mTransformation.column(3).toVector3D();
    mRotation = QQuaternion::fromRotationMatrix(mTransformation.normalMatrix());
    MakeTransformationDirty();
}

void ParticleSimulator::Node::SetPosition(float x, float y, float z)
{
    SetPosition(QVector3D(x, y, z));
}

void ParticleSimulator::Node::SetScale(float x, float y, float z)
{
    SetScale(QVector3D(x, y, z));
}

void ParticleSimulator::Node::RotateGlobal(const QVector3D& axis, float angle)
{
    SetRotation(QQuaternion::fromAxisAndAngle(axis, angle) * GetRotation());
}

void ParticleSimulator::Node::RotateLocal(const QVector3D& axis, float angle)
{
    SetRotation(GetRotation() * QQuaternion::fromAxisAndAngle(axis, angle));
}

void ParticleSimulator::Node::Translate(const QVector3D& delta)
{
    SetPosition(GetPosition() + delta);
}

void ParticleSimulator::Node::UpdateTransformation()
{
    mTransformation.setToIdentity();
    mTransformation.scale(mScale);
    mTransformation.rotate(mRotation);
    mTransformation.setColumn(3, QVector4D(mPosition, 1.0f));

    mNormalMatrix = mTransformation.normalMatrix();
    mTransformationDirty = false;
}