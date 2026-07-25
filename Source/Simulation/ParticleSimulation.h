#pragma once

#include "Renderer/Shader.h"
#include "Util/Macros.h"

#include <QOpenGLFunctions_4_5_Core>
#include <QVector4D>
#include <QVector>

namespace ParticleSimulator
{
    enum class SimulationMode
    {
        Attraction,
        Vortex,
        Explosion,
        Galaxy,
        Fluid,
        Fireworks,
        Swarm,
        BlackHole
    };

    class ParticleSimulation : protected QOpenGLFunctions_4_5_Core
    {
      public:
        ParticleSimulation();

        void Update(float Ifps);
        void Reset();
        void SetSimulationMode(SimulationMode SimulationMode);

      private:
        bool ImportModel(const QString& Path);
        void GenerateRandomGeometry();
        void GenerateGalaxyGeometry();
        void GenerateSphereGeometry();
        void InitializeOpenGLStuff();

        void UpdateAttractors();
        void UpdateParticles(float Ifps);

        Shader* mParticleSimulationShader;

        DEFINE_MEMBER_CONST(GLuint, VertexArrayObject, 0);
        DEFINE_MEMBER_CONST(GLuint, InitialPositionVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, PositionVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, VelocityVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, AttractorsVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, NormalVertexBuffer, 0);
        DEFINE_MEMBER_CONST(GLuint, ParticleDataBuffer, 0);

        DEFINE_MEMBER_CONST(size_t, NumberOfParticles, MAX_NUMBER_OF_PARTICLES);
        DEFINE_MEMBER_CONST(size_t, NumberOfAttractors, NUMBER_OF_ATTRACTORS);
        DEFINE_MEMBER_CONST(float, MaxSpeed, 0.1f);
        DEFINE_MEMBER(float, MaxDistance, 3.0f);

        DEFINE_MEMBER(bool, ApplyAttractionForce, true);
        DEFINE_MEMBER(float, Gravity, -0.5f);
        DEFINE_MEMBER(float, Damping, 0.98f);
        DEFINE_MEMBER(float, TurbulenceStrength, 0.3f);
        DEFINE_MEMBER(float, VortexStrength, 2.0f);
        DEFINE_MEMBER(float, RepulsionStrength, 0.5f);
        DEFINE_MEMBER(float, AttractionStrength, 1.0f);
        DEFINE_MEMBER(float, BoundarySize, 5.0f);
        DEFINE_MEMBER(float, BoundaryBounciness, 0.7f);
        DEFINE_MEMBER(bool, EnableCollisions, false);
        DEFINE_MEMBER(bool, EnableGravity, false);
        DEFINE_MEMBER(bool, EnableTurbulence, true);
        DEFINE_MEMBER(bool, EnableVortex, false);
        DEFINE_MEMBER(bool, EnableBoundary, true);
        DEFINE_MEMBER(SimulationMode, CurrentMode, SimulationMode::Attraction);

        QVector<QVector4D> mInitialPositions;
        QVector<QVector4D> mPositions;
        QVector<QVector4D> mNormals;
        QVector<QVector4D> mVelocities;
        QVector<QVector4D> mParticleData;  // x: age, y: lifetime, z: size, w: type

        QVector<QVector4D> mAttractors;

        float mTime{ 0.0f };

        static constexpr int MAX_NUMBER_OF_PARTICLES{ 1'280'000 };
        static constexpr int NUMBER_OF_ATTRACTORS{ 8 };
    };
}