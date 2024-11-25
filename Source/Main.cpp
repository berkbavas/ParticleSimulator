#include "Renderer/Renderer.h"

#include <QApplication>

using namespace ParticleSimulator;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Renderer renderer;

    renderer.Run();

    return app.exec();
}
