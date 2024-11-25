#pragma once

#include <QByteArray>
#include <QString>

namespace ParticleSimulator
{
    class Util
    {
      public:
        Util() = delete;

        static QByteArray GetBytes(const QString& path);
    };
}
