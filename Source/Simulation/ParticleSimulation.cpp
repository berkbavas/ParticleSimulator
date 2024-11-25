#include "ParticleSimulation.h"

#include "Util/Math.h"

ParticleSimulator::ParticleSimulation::ParticleSimulation(size_t nParticles)
    : mNumberOfParticles(nParticles)
{
    mPositions.resize(mNumberOfParticles);
    mVelocities.resize(mNumberOfParticles);
    mLifes.resize(mNumberOfParticles);

    for (auto& position : mPositions)
    {
        const float x = Math::GenerateRandom(-4, 4);
        const float y = Math::GenerateRandom(-4, 4);
        const float z = Math::GenerateRandom(-4, 4);
        position = QVector3D(x, y, z);
    }

    for (auto& velocity : mVelocities)
    {
        const float x = Math::GenerateRandom(-0.4f, 0.4f);
        const float y = Math::GenerateRandom(-0.4f, 0.4f);
        const float z = Math::GenerateRandom(-0.4f, 0.4f);
        velocity = QVector3D(x, y, z);
    }

    for (auto& life : mLifes)
    {
        life = Math::GenerateRandom(0, 1);
    }

    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    // Position
    glGenBuffers(1, &mPositionVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(QVector3D), mPositions.data(), GL_DYNAMIC_COPY);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*) 0);
    glEnableVertexAttribArray(0);

    // Life
    glGenBuffers(1, &mLifeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mLifeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mLifes.size() * sizeof(float), mLifes.data(), GL_DYNAMIC_COPY);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void ParticleSimulator::ParticleSimulation::Update(float ifps)
{
    // TODO
}
