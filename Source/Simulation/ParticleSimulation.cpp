#include "ParticleSimulation.h"

#include "Util/Math.h"

ParticleSimulator::ParticleSimulation::ParticleSimulation(size_t nParticles, size_t nAttractors)
    : mNumberOfParticles(nParticles)
    , mNumberOfAttractors(nAttractors)
{
    GenerateGeometries();
    InitializeOpenGLStuff();
}

void ParticleSimulator::ParticleSimulation::Update(float ifps)
{
    mTime += ifps;

    UpdateAttractors();
    UpdateParticles(ifps);
}

void ParticleSimulator::ParticleSimulation::UpdateAttractors()
{
    for (int i = 0; i < mNumberOfAttractors; ++i)
    {
        const float x = std::sinf(mTime * (float) (i + 4) * 7.5f * 20.0f) * 50.0f;
        const float y = std::cosf(mTime * (float) (i + 7) * 3.9f * 20.0f) * 50.0f;
        const float z = std::sinf(mTime * (float) (i + 3) * 5.3f * 20.0f) * std::cosf(mTime * (float) (i + 5) * 9.1f) * 100.0f;

        mAttractors[i] = QVector4D(x, y, z, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mAttractorsVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mNumberOfAttractors * sizeof(QVector4D), mAttractors.data());
}

void ParticleSimulator::ParticleSimulation::UpdateParticles(float ifps)
{
    mParticleSimulationShader->Bind();
    mParticleSimulationShader->SetUniformValue("dt", ifps);
    mParticleSimulationShader->SetUniformValue("numberOfAttractors", static_cast<int>(mNumberOfAttractors));
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mPositionVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mVelocityVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, mAttractorsVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, mLifeVertexBuffer);
    glDispatchCompute(mNumberOfParticles / 128, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    mParticleSimulationShader->Release();
}

void ParticleSimulator::ParticleSimulation::GenerateGeometries()
{
    mPositions.resize(mNumberOfParticles);
    mVelocities.resize(mNumberOfParticles);
    mLifes.resize(mNumberOfParticles);
    mAttractors.resize(mNumberOfAttractors);

    for (auto& position : mPositions)
    {
        const float x = Math::GenerateRandom(-1, 1);
        const float y = Math::GenerateRandom(-1, 1);
        const float z = Math::GenerateRandom(-1, 1);
        position = mMaxDistance * QVector4D(x, y, z, 1);
    }

    for (auto& velocity : mVelocities)
    {
        const float x = Math::GenerateRandom(-1, 1);
        const float y = Math::GenerateRandom(-1, 1);
        const float z = Math::GenerateRandom(-1, 1);
        velocity = mMaxSpeed * QVector4D(x, y, z, 1);
    }

    for (auto& life : mLifes)
    {
        life = mMaxLife * Math::GenerateRandom(0, 1);
    }

    for (auto& attractor : mAttractors)
    {
        attractor = QVector4D(0, 0, 0, 1);
    }
}

void ParticleSimulator::ParticleSimulation::InitializeOpenGLStuff()
{
    mParticleSimulationShader = new Shader("Particle Simulation Shader");
    mParticleSimulationShader->AddPath(QOpenGLShader::Compute, ":/Resources/Shaders/ParticleSimulation.comp");
    mParticleSimulationShader->Initialize();

    initializeOpenGLFunctions();

    // Position
    glGenBuffers(1, &mPositionVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(QVector4D), mPositions.data(), GL_DYNAMIC_COPY);

    // Velocities
    glGenBuffers(1, &mVelocityVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVelocityVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVelocities.size() * sizeof(QVector4D), mVelocities.data(), GL_DYNAMIC_COPY);

    // Life
    glGenBuffers(1, &mLifeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mLifeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mLifes.size() * sizeof(float), mLifes.data(), GL_DYNAMIC_COPY);

    // Attractors
    glGenBuffers(1, &mAttractorsVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mAttractorsVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mAttractors.size() * sizeof(QVector4D), mAttractors.data(), GL_DYNAMIC_COPY);

    /// Rendering
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    /// Position
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*) 0);
    glEnableVertexAttribArray(0);

    /// Life
    glBindBuffer(GL_ARRAY_BUFFER, mLifeVertexBuffer);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
