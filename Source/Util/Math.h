#pragma once

#include <QByteArray>
#include <QQuaternion>
#include <QString>

#include <QRandomGenerator>

namespace ParticleSimulator
{
    class Math
    {
      public:
        Math() = delete;

        static void GetEulerDegrees(const QQuaternion& rotation, float& yaw, float& pitch, float& roll);
        static QQuaternion ConstructFromEulerDegrees(float yaw, float pitch, float roll);

        static float AngleBetween(const QVector3D& v1, const QVector3D& v2, const QVector3D& left);
        static float AngleBetween(const QVector3D& v1, const QVector3D& v2);
        static float GenerateRandom(float lowerBound, float upperBound);


        static QRandomGenerator RANDOM_GENERATOR;
    };
}
