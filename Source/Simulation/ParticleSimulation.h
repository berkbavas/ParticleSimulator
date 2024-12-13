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
        ParticleSimulation(size_t nParticles, size_t nAttractors);

        void Update(float ifps);

      private:
        void GenerateGeometries();
        void InitializeOpenGLStuff();

        void UpdateAttractors();
        void UpdateParticles(float ifps);

        Shader* mParticleSimulationShader;

        DEFINE_MEMBER_CONST(GLuint, VertexArrayObject, 0);
        DEFINE_MEMBER_CONST(GLuint, PositionVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, VelocityVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, LifeVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, AttractorsVertexBuffer, 0);

        DEFINE_MEMBER_CONST(size_t, NumberOfParticles, 0);
        DEFINE_MEMBER_CONST(size_t, NumberOfAttractors, 0);
        DEFINE_MEMBER_CONST(float, MaxLife, 1.0f);
        DEFINE_MEMBER_CONST(float, MaxSpeed, 0.1f);
        DEFINE_MEMBER_CONST(float, MaxDistance, 10.0f);

        QVector<QVector4D> mPositions;
        QVector<QVector4D> mVelocities;
        QVector<float> mLifes;

        QVector<QVector4D> mAttractors;

        float mTime{ 0.0f };
    };
}