#include "Shader.h"

#include "Util/Util.h"

#include <QDebug>
#include <QFile>

ParticleSimulator::Shader::Shader(const QString& name)
    : mProgram(nullptr)
    , mName(name)
{}

void ParticleSimulator::Shader::Initialize()
{
    LOG_INFO("Shader::Initialize: '{}' is being initializing.", mName.toStdString());

    initializeOpenGLFunctions();

    mProgram = QSharedPointer<QOpenGLShaderProgram>(new QOpenGLShaderProgram);

    for (const auto [shaderType, path] : mPaths)
    {
        const auto bytes = Util::GetBytes(path);

        if (!mProgram->addShaderFromSourceCode(shaderType, bytes))
        {
            PS_EXIT_FAILURE("Shader::Initialize: '{}' could not be loaded.", GetShaderTypeString(shaderType).toStdString());
        }
    }

    if (!mProgram->link())
    {
        PS_EXIT_FAILURE("Shader::Initialize: Could not link shader program.");
    }

    if (!mProgram->bind())
    {
        PS_EXIT_FAILURE("Shader::Initialize: Could not bind shader program.");
    }

    LOG_INFO("Shader::Initialize: '{}' has been initialized.", mName.toStdString());
}

bool ParticleSimulator::Shader::Bind()
{
    return mProgram->bind();
}

void ParticleSimulator::Shader::Release()
{
    mProgram->release();
}

void ParticleSimulator::Shader::AddPath(QOpenGLShader::ShaderTypeBit type, const QString& path)
{
    mPaths.emplace(type, path);
}

QString ParticleSimulator::Shader::GetName() const
{
    return mName;
}

QString ParticleSimulator::Shader::GetShaderTypeString(QOpenGLShader::ShaderTypeBit type)
{
    switch (type)
    {
    case QOpenGLShader::Vertex:
        return "Vertex Shader";
    case QOpenGLShader::Fragment:
        return "Fragment Shader";
    case QOpenGLShader::Geometry:
        return "Geometry Shader";
    case QOpenGLShader::TessellationControl:
        return "Tessellation Control Shader";
    case QOpenGLShader::TessellationEvaluation:
        return "Tesselation Evaluation Shader";
    case QOpenGLShader::Compute:
        return "Compute Shader";
    default:
        return "Unknown Shader";
    }
}

void ParticleSimulator::Shader::SetSampler(const QString& name, GLuint unit, GLuint textureId, GLuint target)
{
    SetUniformValue(name, unit);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, textureId);
}
