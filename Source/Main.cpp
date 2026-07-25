#include "Renderer/Renderer.h"

#include <QApplication>

int main(int Argc, char* Argv[])
{
    QApplication Application(Argc, Argv);
    ParticleSimulator::Renderer Renderer;
    Renderer.Run();
    return Application.exec();
}
