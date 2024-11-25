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

        QVector<QVector3D> mPositions;
        QVector<QVector3D> mVelocities;
        QVector<float> mLifes;

        QVector<QVector3D> mAttractors;

        float mTime{ 0.0f };
    };
}