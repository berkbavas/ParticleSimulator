#pragma once

#include "Util/Macros.h"

#include <memory>
#include <set>

#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>
#include <QVector3D>
#include <QtMath>

namespace ParticleSimulator
{
    class Node
    {
        DISABLE_COPY(Node);

      protected:
        Node() = default;
        virtual ~Node() = default;

      public:
        const QMatrix4x4& GetTransformation();

        const QQuaternion& GetRotation() const { return mRotation; }
        const QVector3D& GetPosition() const { return mPosition; }
        const QVector3D& GetScale() const { return mScale; }
        const QMatrix3x3 GetNormalMatrix() const { return mNormalMatrix; }

        void SetTransformation(const QMatrix4x4& newTransformation);
        void SetRotation(const QQuaternion& newRotation);
        void SetPosition(const QVector3D& newPosition);
        void SetPosition(float x, float y, float z);
        void SetScale(const QVector3D& newScale);
        void SetScale(float x, float y, float z);

        void RotateGlobal(const QVector3D& axis, float angle);
        void RotateLocal(const QVector3D& axis, float angle);
        void Translate(const QVector3D& delta);

        void MakeTransformationDirty() { mTransformationDirty = true; }
        bool GetTransformationDirty() const { return mTransformationDirty; }
        void UpdateTransformation();

      private:
        QMatrix4x4 mTransformation;
        QMatrix3x3 mNormalMatrix;
        QQuaternion mRotation;
        QVector3D mPosition{ QVector3D(0, 0, 0) };
        QVector3D mScale{ QVector3D(1, 1, 1) };

        // For caching
        QMatrix4x4 mWorldTransformation;
        QQuaternion mWorldRotation;
        QVector3D mWorldPosition;

        bool mTransformationDirty{ false };

        DEFINE_MEMBER(bool, Visible, true);
    };

    using NodePtr = std::shared_ptr<Node>;

}
