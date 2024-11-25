#include "ParticleSimulation.h"

#include "Util/Math.h"

ParticleSimulator::ParticleSimulation::ParticleSimulation(size_t nParticles, size_t nAttractors)
    : mNumberOfParticles(nParticles)
    , mNumberOfAttractors(nAttractors)
{
    GenerateGeometries();
    InitializeOpenGLStuff();

    mParticleSimulationShader = new Shader("Particle Simulation Shader");
    mParticleSimulationShader->AddPath(QOpenGLShader::Compute, ":/Resources/Shaders/ParticleSimulation.comp");
    mParticleSimulationShader->Initialize();
}

void ParticleSimulator::ParticleSimulation::Update(float ifps)
{
    mTime += ifps;

    if(mTime > 10.0f)
    {
        mTime = 0.0f;
    }

    UpdateAttractors();
    UpdateParticles(ifps);
}

void ParticleSimulator::ParticleSimulation::UpdateAttractors()
{
    for (int i = 0; i < mNumberOfAttractors; ++i)
    {
        const float x = std::sinf(mTime) * Math::GenerateRandom(-25, 25);
        const float y = std::cosf(mTime) * Math::GenerateRandom(-25, 25);
        const float z = std::tanf(mTime);

        mAttractors[i] = QVector3D(x, y, z);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mAttractorsVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mNumberOfAttractors * sizeof(QVector3D), mAttractors.data());
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
        const float x = Math::GenerateRandom(-2, 2);
        const float y = Math::GenerateRandom(-2, 2);
        const float z = Math::GenerateRandom(-2, 2);
        position = QVector3D(x, y, z);
    }

    for (auto& velocity : mVelocities)
    {
        const float x = Math::GenerateRandom(-0.1f, 0.1f);
        const float y = Math::GenerateRandom(-0.1f, 0.1f);
        const float z = Math::GenerateRandom(-0.1f, 0.1f);
        velocity = QVector3D(x, y, z);
    }

    for (auto& life : mLifes)
    {
        life = Math::GenerateRandom(0, 1);
    }

    for (auto& attractor : mAttractors)
    {
        const float x = Math::GenerateRandom(-15, 15);
        const float y = Math::GenerateRandom(-15, 15);
        const float z = Math::GenerateRandom(-15, 15);
        attractor = QVector3D(x, y, z);
    }
}

void ParticleSimulator::ParticleSimulation::InitializeOpenGLStuff()
{
    initializeOpenGLFunctions();

    // Position
    glGenBuffers(1, &mPositionVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(QVector3D), mPositions.data(), GL_DYNAMIC_COPY);

    // Velocities
    glGenBuffers(1, &mVelocityVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVelocityVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVelocities.size() * sizeof(QVector3D), mVelocities.data(), GL_DYNAMIC_COPY);

    // Life
    glGenBuffers(1, &mLifeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mLifeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mLifes.size() * sizeof(float), mLifes.data(), GL_DYNAMIC_COPY);

    // Attractors
    glGenBuffers(1, &mAttractorsVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mAttractorsVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mAttractors.size() * sizeof(QVector3D), mAttractors.data(), GL_DYNAMIC_COPY);

    /// Rendering
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    /// Position
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*) 0);
    glEnableVertexAttribArray(0);

    /// Life
    glBindBuffer(GL_ARRAY_BUFFER, mLifeVertexBuffer);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
