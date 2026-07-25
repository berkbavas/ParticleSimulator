#include "Shader.h"

#include "Util/Util.h"

#include <QDebug>
#include <QFile>

ParticleSimulator::Shader::Shader(const QString& Name)
    : mProgram(nullptr)
    , mName(Name)
{
    LOG_TRACE("Shader::Shader: '{}' is being constructed.", mName.toStdString());
    initializeOpenGLFunctions();
    LOG_TRACE("Shader::Shader: '{}' has been constructed.", mName.toStdString());
}

void ParticleSimulator::Shader::Initialize()
{
    LOG_INFO("Shader::Initialize: '{}' is being initializing.", mName.toStdString());

    mProgram = std::make_unique<QOpenGLShaderProgram>();

    for (const auto& [ShaderType, Path] : mPaths)
    {
        const auto Bytes = ParticleSimulator::Util::GetBytes(Path);

        if (Bytes.size() == 0)
        {
            PS_EXIT_FAILURE("Shader::Initialize: '{}' could not be found: '{}'", GetShaderTypeString(ShaderType).toStdString(), mName.toStdString());
        }

        if (!mProgram->addShaderFromSourceCode(ShaderType, Bytes))
        {
            PS_EXIT_FAILURE("Shader::Initialize: '{}' could not be loaded: '{}'", GetShaderTypeString(ShaderType).toStdString(), mName.toStdString());
        }
    }

    if (mCallback != nullptr)
    {
        LOG_DEBUG("Shader::Initialize: Calling callback...");
        mCallback(QOpenGLContext::currentContext(), mProgram.get());
        LOG_DEBUG("Shader::Initialize: Callback returned.");
    }

    if (!mProgram->link())
    {
        PS_EXIT_FAILURE("Shader::Initialize: Could not link shader program: '{}'", mName.toStdString());
    }

    if (!mProgram->bind())
    {
        PS_EXIT_FAILURE("Shader::Initialize: Could not bind shader program: '{}'", mName.toStdString());
    }

    LOG_INFO("Shader::Initialize: '{}' has been initialized.", mName.toStdString());
}

bool ParticleSimulator::Shader::Bind()
{
    return mProgram->bind();
}

void ParticleSimulator::Shader::Unbind()
{
    mProgram->release();
}

void ParticleSimulator::Shader::SetCallbackBeforeLinking(const Callback& Callback)
{
    mCallback = Callback;
}

void ParticleSimulator::Shader::AddPath(QOpenGLShader::ShaderTypeBit Type, const QString& Path)
{
    mPaths.emplace(Type, Path);
}

QString ParticleSimulator::Shader::GetName() const
{
    return mName;
}

QString ParticleSimulator::Shader::GetShaderTypeString(QOpenGLShader::ShaderTypeBit Type)
{
    switch (Type)
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
        return "Tessellation Evaluation Shader";
    case QOpenGLShader::Compute:
        return "Compute Shader";
    default:
        return "Unknown Shader";
    }
}

void ParticleSimulator::Shader::SetSampler(const QString& Name, GLuint Unit, GLuint TextureId, GLuint Target)
{
    SetUniform(Name, Unit);
    glActiveTexture(GL_TEXTURE0 + Unit);
    glBindTexture(Target, TextureId);
}
