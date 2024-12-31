#include "ParticleSimulation.h"

#include "Util/Math.h"

#include <QFile>

ParticleSimulator::ParticleSimulation::ParticleSimulation()
{
    if (ImportModel("Resources/Model.obj"))
    {
        LOG_INFO("ParticleSimulation::ParticleSimulation: Model has been imported.");
    }
    else
    {
        LOG_WARN("ParticleSimulation::ParticleSimulation: Model could not be imported. Generrating a random geometry.");
        GenerateRandomGeometry();
    }

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
        const float x = std::sinf(mTime * (float) (i + 4) * 7.5f * 20.0f) * mMaxDistance;
        const float y = std::cosf(mTime * (float) (i + 7) * 3.9f * 20.0f) * mMaxDistance;
        const float z = std::sinf(mTime * (float) (i + 3) * 5.3f * 20.0f) * std::cosf(mTime * (float) (i + 5) * 9.1f) * mMaxDistance;

        mAttractors[i] = QVector4D(x, y, z, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mAttractorsVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mNumberOfAttractors * sizeof(QVector4D), mAttractors.data());
}

void ParticleSimulator::ParticleSimulation::UpdateParticles(float ifps)
{
    mParticleSimulationShader->Bind();
    mParticleSimulationShader->SetUniformValue("dt", ifps);
    mParticleSimulationShader->SetUniformValue("applyAttractionForce", mApplyAttractionForce);
    mParticleSimulationShader->SetUniformValue("numberOfAttractors", static_cast<int>(mNumberOfAttractors));

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mInitialPositionVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mPositionVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, mVelocityVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, mAttractorsVertexBuffer);
    glDispatchCompute(mNumberOfParticles / 128, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    mParticleSimulationShader->Release();
}

bool ParticleSimulator::ParticleSimulation::ImportModel(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        if (fields.size() < 2)
        {
            continue;
        }

        if (fields[0] == "v")
        {
            mInitialPositions.emplace_back(fields[1].toDouble(), fields[2].toDouble(), fields[3].toDouble(), 1.0f);
            mPositions.emplace_back(fields[1].toDouble(), fields[2].toDouble(), fields[3].toDouble(), 1.0f);
        }

        if (fields[0] == "vn")
        {
            mNormals.emplace_back(fields[1].toDouble(), fields[2].toDouble(), fields[3].toDouble(), 1.0f);
        }
    }

    file.close();

    mVelocities.resize(mPositions.size());

    for (auto& velocity : mVelocities)
    {
        velocity = QVector4D(0, 0, 0, 1);
    }

    mAttractors.resize(mNumberOfAttractors);

    for (auto& attractor : mAttractors)
    {
        attractor = QVector4D(0, 0, 0, 1);
    }

    return true;
}

void ParticleSimulator::ParticleSimulation::GenerateRandomGeometry()
{
    mInitialPositions.resize(mNumberOfParticles);
    mPositions.resize(mNumberOfParticles);
    mNormals.resize(mNumberOfParticles);
    mVelocities.resize(mNumberOfParticles);
    mAttractors.resize(mNumberOfAttractors);

    for (auto& position : mInitialPositions)
    {
        const float x = Math::GenerateRandom(-1, 1);
        const float y = std::sin(M_PI * x);
        const float z = Math::GenerateRandom(-1, 1);
        position = mMaxDistance * QVector4D(x, y, z, 1);
    }

    for (auto& position : mPositions)
    {
        position = mMaxDistance * QVector4D(0, 0, 0, 1);
    }

    for (auto& normal : mNormals)
    {
        normal = QVector4D(0, 1, 0, 0);
    }

    for (auto& velocity : mVelocities)
    {
        const float x = Math::GenerateRandom(-1, 1);
        const float y = Math::GenerateRandom(-1, 1);
        const float z = Math::GenerateRandom(-1, 1);
        velocity = mMaxSpeed * QVector4D(x, y, z, 1);
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

    // Initial Position
    glGenBuffers(1, &mInitialPositionVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mInitialPositionVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mInitialPositions.size() * sizeof(QVector4D), mInitialPositions.data(), GL_DYNAMIC_COPY);

    // Position
    glGenBuffers(1, &mPositionVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(QVector4D), mPositions.data(), GL_DYNAMIC_COPY);

    // Velocities
    glGenBuffers(1, &mVelocityVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVelocityVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVelocities.size() * sizeof(QVector4D), mVelocities.data(), GL_DYNAMIC_COPY);

    // Attractors
    glGenBuffers(1, &mAttractorsVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mAttractorsVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mAttractors.size() * sizeof(QVector4D), mAttractors.data(), GL_DYNAMIC_COPY);

    /// Rendering
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    /// Positions
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*) 0);
    glEnableVertexAttribArray(0);

    /// Initial Positions
    glBindBuffer(GL_ARRAY_BUFFER, mInitialPositionVertexBuffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*) 0);
    glEnableVertexAttribArray(1);

    /// Normals
    glGenBuffers(1, &mNormalVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mNormalVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(QVector4D), mNormals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*) 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
