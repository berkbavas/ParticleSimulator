#include "ParticleSimulation.h"

#include "Util/Math.h"

#include <cmath>

#include <QFile>

ParticleSimulator::ParticleSimulation::ParticleSimulation()
{
    if (ImportModel("Resources/Model.obj"))
    {
        LOG_INFO("ParticleSimulation::ParticleSimulation: Model has been imported.");
    }
    else
    {
        LOG_WARN("ParticleSimulation::ParticleSimulation: Model could not be imported. Generating galaxy geometry.");
        GenerateGalaxyGeometry();
    }

    InitializeOpenGLStuff();
}

void ParticleSimulator::ParticleSimulation::Update(float Ifps)
{
    mTime += Ifps;

    UpdateAttractors();
    UpdateParticles(Ifps);
}

void ParticleSimulator::ParticleSimulation::Reset()
{
    mTime = 0.0f;

    for (size_t Index = 0; Index < mPositions.size(); ++Index)
    {
        mPositions[Index] = mInitialPositions[Index];
        mVelocities[Index] = QVector4D(0, 0, 0, 1);
        mParticleData[Index].setX(0.0f);
    }
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mPositions.size() * sizeof(QVector4D), mPositions.data());

    glBindBuffer(GL_ARRAY_BUFFER, mVelocityVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mVelocities.size() * sizeof(QVector4D), mVelocities.data());

    glBindBuffer(GL_ARRAY_BUFFER, mParticleDataBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mParticleData.size() * sizeof(QVector4D), mParticleData.data());
}

void ParticleSimulator::ParticleSimulation::SetSimulationMode(SimulationMode SimulationMode)
{
    mCurrentMode = SimulationMode;

    switch (SimulationMode)
    {
    case SimulationMode::Attraction:
        mEnableGravity = false;
        mEnableTurbulence = true;
        mEnableVortex = false;
        mApplyAttractionForce = true;
        mAttractionStrength = 1.0f;
        break;

    case SimulationMode::Vortex:
        mEnableGravity = false;
        mEnableTurbulence = true;
        mEnableVortex = true;
        mApplyAttractionForce = true;
        mVortexStrength = 3.0f;
        break;

    case SimulationMode::Explosion:
        mEnableGravity = true;
        mEnableTurbulence = true;
        mEnableVortex = false;
        mApplyAttractionForce = false;
        mGravity = -2.0f;
        break;

    case SimulationMode::Galaxy:
        mEnableGravity = false;
        mEnableTurbulence = false;
        mEnableVortex = true;
        mApplyAttractionForce = true;
        mVortexStrength = 1.5f;
        mAttractionStrength = 0.5f;
        break;

    case SimulationMode::Fluid:
        mEnableGravity = true;
        mEnableTurbulence = true;
        mEnableVortex = false;
        mEnableCollisions = true;
        mApplyAttractionForce = false;
        mGravity = -1.0f;
        mDamping = 0.95f;
        break;

    case SimulationMode::Fireworks:
        mEnableGravity = true;
        mEnableTurbulence = false;
        mEnableVortex = false;
        mApplyAttractionForce = false;
        mGravity = -3.0f;
        break;

    case SimulationMode::Swarm:
        mEnableGravity = false;
        mEnableTurbulence = true;
        mEnableVortex = false;
        mApplyAttractionForce = true;
        mRepulsionStrength = 0.8f;
        mAttractionStrength = 0.3f;
        break;

    case SimulationMode::BlackHole:
        mEnableGravity = false;
        mEnableTurbulence = false;
        mEnableVortex = true;
        mApplyAttractionForce = true;
        mVortexStrength = 5.0f;
        mAttractionStrength = 3.0f;
        break;
    }
}

void ParticleSimulator::ParticleSimulation::UpdateAttractors()
{
    for (int Index = 0; Index < mNumberOfAttractors; ++Index)
    {
        float PhaseOffset = static_cast<float>(Index) * 0.785398f; // PI/4
        float SpeedMult = 1.0f + static_cast<float>(Index) * 0.1f;

        float X, Y, Z;

        switch (mCurrentMode)
        {
        case SimulationMode::Galaxy:
        case SimulationMode::BlackHole:
            X = std::sin(mTime * SpeedMult + PhaseOffset) * mMaxDistance * 0.3f;
            Y = std::cos(mTime * SpeedMult * 0.5f + PhaseOffset) * mMaxDistance * 0.1f;
            Z = std::cos(mTime * SpeedMult + PhaseOffset) * mMaxDistance * 0.3f;
            break;

        case SimulationMode::Vortex:
            X = std::sin(mTime * 2.0f + PhaseOffset) * mMaxDistance * (0.5f + 0.5f * std::sin(mTime * 0.5f));
            Y = std::sin(mTime * 0.7f) * mMaxDistance * 0.5f;
            Z = std::cos(mTime * 2.0f + PhaseOffset) * mMaxDistance * (0.5f + 0.5f * std::sin(mTime * 0.5f));
            break;

        case SimulationMode::Swarm:
            X = std::sin(mTime * (Index + 4) * 0.37f) * mMaxDistance;
            Y = std::cos(mTime * (Index + 7) * 0.19f) * mMaxDistance;
            Z = std::sin(mTime * (Index + 3) * 0.53f) * std::cos(mTime * (Index + 5) * 0.91f) * mMaxDistance;
            break;

        default:
            X = std::sin(mTime * (Index + 4) * 7.5f * 20.0f) * mMaxDistance;
            Y = std::cos(mTime * (Index + 7) * 3.9f * 20.0f) * mMaxDistance;
            Z = std::sin(mTime * (Index + 3) * 5.3f * 20.0f) * std::cos(mTime * (Index + 5) * 9.1f) * mMaxDistance;
            break;
        }

        mAttractors[Index] = QVector4D(X, Y, Z, mAttractionStrength);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mAttractorsVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mNumberOfAttractors * sizeof(QVector4D), mAttractors.data());
}

void ParticleSimulator::ParticleSimulation::UpdateParticles(float Ifps)
{
    mParticleSimulationShader->Bind();

    mParticleSimulationShader->SetUniform("uDt", Ifps);
    mParticleSimulationShader->SetUniform("uTime", mTime);
    mParticleSimulationShader->SetUniform("uApplyAttractionForce", mApplyAttractionForce);
    mParticleSimulationShader->SetUniform("uNumberOfAttractors", static_cast<int>(mNumberOfAttractors));

    mParticleSimulationShader->SetUniform("uGravity", mGravity);
    mParticleSimulationShader->SetUniform("uDamping", mDamping);
    mParticleSimulationShader->SetUniform("uTurbulenceStrength", mTurbulenceStrength);
    mParticleSimulationShader->SetUniform("uVortexStrength", mVortexStrength);
    mParticleSimulationShader->SetUniform("uRepulsionStrength", mRepulsionStrength);
    mParticleSimulationShader->SetUniform("uAttractionStrength", mAttractionStrength);
    mParticleSimulationShader->SetUniform("uBoundarySize", mBoundarySize);
    mParticleSimulationShader->SetUniform("uBoundaryBounciness", mBoundaryBounciness);

    mParticleSimulationShader->SetUniform("uEnableGravity", mEnableGravity);
    mParticleSimulationShader->SetUniform("uEnableTurbulence", mEnableTurbulence);
    mParticleSimulationShader->SetUniform("uEnableVortex", mEnableVortex);
    mParticleSimulationShader->SetUniform("uEnableBoundary", mEnableBoundary);
    mParticleSimulationShader->SetUniform("uSimulationMode", static_cast<int>(mCurrentMode));

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mInitialPositionVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mPositionVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, mVelocityVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, mAttractorsVertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, mParticleDataBuffer);

    glDispatchCompute(mNumberOfParticles / 128, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    mParticleSimulationShader->Unbind();
}

bool ParticleSimulator::ParticleSimulation::ImportModel(const QString& Path)
{
    QFile File(Path);
    if (!File.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream TextStream(&File);

    while (!TextStream.atEnd())
    {
        QString Line = TextStream.readLine();
        QStringList Fields = Line.split(" ");
        if (Fields.size() < 2)
        {
            continue;
        }

        if (Fields[0] == "v")
        {
            mInitialPositions.emplace_back(Fields[1].toDouble(), Fields[2].toDouble(), Fields[3].toDouble(), 1.0f);
            mPositions.emplace_back(Fields[1].toDouble(), Fields[2].toDouble(), Fields[3].toDouble(), 1.0f);
        }

        if (Fields[0] == "vn")
        {
            mNormals.emplace_back(Fields[1].toDouble(), Fields[2].toDouble(), Fields[3].toDouble(), 1.0f);
        }
    }

    File.close();

    mVelocities.resize(mPositions.size());
    mParticleData.resize(mPositions.size());

    for (size_t Index = 0; Index < mVelocities.size(); ++Index)
    {
        mVelocities[Index] = QVector4D(0, 0, 0, 1);
        float Lifetime = 5.0f + Math::GenerateRandom(0, 10.0f);
        mParticleData[Index] = QVector4D(0, Lifetime, 1.0f, 0);
    }

    mAttractors.resize(mNumberOfAttractors);

    for (auto& Attractor : mAttractors)
    {
        Attractor = QVector4D(0, 0, 0, 1);
    }

    return true;
}

void ParticleSimulator::ParticleSimulation::GenerateRandomGeometry()
{
    mInitialPositions.resize(mNumberOfParticles);
    mPositions.resize(mNumberOfParticles);
    mNormals.resize(mNumberOfParticles);
    mVelocities.resize(mNumberOfParticles);
    mParticleData.resize(mNumberOfParticles);
    mAttractors.resize(mNumberOfAttractors);

    for (size_t Index = 0; Index < mNumberOfParticles; ++Index)
    {
        const float X = Math::GenerateRandom(-1, 1);
        const float Y = std::sin(M_PI * X);
        const float Z = Math::GenerateRandom(-1, 1);
        mInitialPositions[Index] = mMaxDistance * QVector4D(X, Y, Z, 1);
        mPositions[Index] = mInitialPositions[Index];
        mNormals[Index] = QVector4D(0, 1, 0, 0);

        const float Vx = Math::GenerateRandom(-1, 1);
        const float Vy = Math::GenerateRandom(-1, 1);
        const float Vz = Math::GenerateRandom(-1, 1);
        mVelocities[Index] = mMaxSpeed * QVector4D(Vx, Vy, Vz, 1);

        const float Lifetime = 5.0f + Math::GenerateRandom(0, 10.0f);
        mParticleData[Index] = QVector4D(0, Lifetime, 1.0f, 0);
    }

    for (auto& Attractor : mAttractors)
    {
        Attractor = QVector4D(0, 0, 0, 1);
    }
}

void ParticleSimulator::ParticleSimulation::GenerateGalaxyGeometry()
{
    mInitialPositions.resize(mNumberOfParticles);
    mPositions.resize(mNumberOfParticles);
    mNormals.resize(mNumberOfParticles);
    mVelocities.resize(mNumberOfParticles);
    mParticleData.resize(mNumberOfParticles);
    mAttractors.resize(mNumberOfAttractors);

    const float ArmCount = 4.0f;
    const float ArmSpread = 0.5f;
    const float MaxRadius = mMaxDistance;

    for (size_t Index = 0; Index < mNumberOfParticles; ++Index)
    {
        float T = static_cast<float>(Index) / static_cast<float>(mNumberOfParticles);

        float ArmIndex = std::floor(Math::GenerateRandom(0, ArmCount));
        float ArmAngle = (ArmIndex / ArmCount) * 2.0f * M_PI;

        float Radius = Math::GenerateRandom(0.1f, 1.0f) * MaxRadius;
        float SpiralAngle = (1.0f - Radius / MaxRadius) * 3.0f * M_PI;

        float Angle = ArmAngle + SpiralAngle + Math::GenerateRandom(-ArmSpread, ArmSpread);

        float X = std::cos(Angle) * Radius;
        float Z = std::sin(Angle) * Radius;
        float Y = Math::GenerateRandom(-0.1f, 0.1f) * (1.0f - Radius / MaxRadius);

        mInitialPositions[Index] = QVector4D(X, Y, Z, 1);
        mPositions[Index] = mInitialPositions[Index];

        mNormals[Index] = QVector4D(0, 1, 0, 0);

        float OrbitalSpeed = std::sqrt(1.0f / (Radius + 0.1f)) * 0.5f;
        float Vx = -std::sin(Angle) * OrbitalSpeed;
        float Vz = std::cos(Angle) * OrbitalSpeed;
        mVelocities[Index] = QVector4D(Vx, 0, Vz, 1);

        float Lifetime = 10.0f + Math::GenerateRandom(0, 20.0f);
        float Size = 0.5f + Math::GenerateRandom(0, 1.0f);
        float Type = (Radius < MaxRadius * 0.2f) ? 1.0f : 0.0f;
        mParticleData[Index] = QVector4D(0, Lifetime, Size, Type);
    }

    for (auto& Attractor : mAttractors)
    {
        Attractor = QVector4D(0, 0, 0, 1);
    }

    mAttractors[0] = QVector4D(0, 0, 0, 2.0f);
}

void ParticleSimulator::ParticleSimulation::GenerateSphereGeometry()
{
    mInitialPositions.resize(mNumberOfParticles);
    mPositions.resize(mNumberOfParticles);
    mNormals.resize(mNumberOfParticles);
    mVelocities.resize(mNumberOfParticles);
    mParticleData.resize(mNumberOfParticles);
    mAttractors.resize(mNumberOfAttractors);

    for (size_t Index = 0; Index < mNumberOfParticles; ++Index)
    {
        float Phi = std::acos(1.0f - 2.0f * (static_cast<float>(Index) + 0.5f) / static_cast<float>(mNumberOfParticles));
        float Theta = M_PI * (1.0f + std::sqrt(5.0f)) * static_cast<float>(Index);

        float Radius = mMaxDistance * (0.5f + 0.5f * Math::GenerateRandom(0, 1));

        float X = std::sin(Phi) * std::cos(Theta) * Radius;
        float Y = std::sin(Phi) * std::sin(Theta) * Radius;
        float Z = std::cos(Phi) * Radius;

        mInitialPositions[Index] = QVector4D(X, Y, Z, 1);
        mPositions[Index] = mInitialPositions[Index];

        QVector3D Normal = QVector3D(X, Y, Z).normalized();
        mNormals[Index] = QVector4D(Normal.x(), Normal.y(), Normal.z(), 0);

        mVelocities[Index] = QVector4D(0, 0, 0, 1);

        float Lifetime = 8.0f + Math::GenerateRandom(0, 12.0f);
        mParticleData[Index] = QVector4D(0, Lifetime, 1.0f, 0);
    }

    for (auto& Attractor : mAttractors)
    {
        Attractor = QVector4D(0, 0, 0, 1);
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

    // Particle Data
    glGenBuffers(1, &mParticleDataBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mParticleDataBuffer);
    glBufferData(GL_ARRAY_BUFFER, mParticleData.size() * sizeof(QVector4D), mParticleData.data(), GL_DYNAMIC_COPY);

    // Rendering
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVertexBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*) 0);
    glEnableVertexAttribArray(0);

    // Initial Positions
    glBindBuffer(GL_ARRAY_BUFFER, mInitialPositionVertexBuffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*) 0);
    glEnableVertexAttribArray(1);

    // Normals
    glGenBuffers(1, &mNormalVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mNormalVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(QVector4D), mNormals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*) 0);
    glEnableVertexAttribArray(2);

    /// Particle Data
    glBindBuffer(GL_ARRAY_BUFFER, mParticleDataBuffer);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*) 0);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}
