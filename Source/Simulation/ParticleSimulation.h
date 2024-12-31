#pragma once

#include "Renderer/Shader.h"
#include "Util/Macros.h"

#include <QOpenGLFunctions_4_3_Core>
#include <QVector4D>
#include <QVector>

namespace ParticleSimulator
{
    class ParticleSimulation : protected QOpenGLFunctions_4_3_Core
    {
      public:
        ParticleSimulation();

        void Update(float ifps);

      private:
        bool ImportModel(const QString& path);
        void GenerateRandomGeometry();
        void InitializeOpenGLStuff();

        void UpdateAttractors();
        void UpdateParticles(float ifps);

        Shader* mParticleSimulationShader;

        DEFINE_MEMBER_CONST(GLuint, VertexArrayObject, 0);
        DEFINE_MEMBER_CONST(GLuint, InitialPositionVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, PositionVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, VelocityVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, AttractorsVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, NormalVertexBuffer, 0);

        DEFINE_MEMBER_CONST(size_t, NumberOfParticles, MAX_NUMBER_OF_PARTICLES);
        DEFINE_MEMBER_CONST(size_t, NumberOfAttractors, NUMBER_OF_ATTRACTORS);
        DEFINE_MEMBER_CONST(float, MaxSpeed, 0.1f);
        DEFINE_MEMBER_CONST(float, MaxDistance, 3.0f);

        DEFINE_MEMBER(bool, ApplyAttractionForce, false);

        QVector<QVector4D> mInitialPositions;
        QVector<QVector4D> mPositions;
        QVector<QVector4D> mNormals;
        QVector<QVector4D> mVelocities;

        QVector<QVector4D> mAttractors;

        float mTime{ 0.0f };

        static constexpr int MAX_NUMBER_OF_PARTICLES{ 1'280'000 };
        static constexpr int NUMBER_OF_ATTRACTORS{ 8 };
    };
}