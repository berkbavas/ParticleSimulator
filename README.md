# Particle Simulator

**Particle Simulator** is a GPU-accelerated particle simulation and visualization tool, ported to Qt from [Maurice Tollmien's original project](https://github.com/MauriceGit/Partikel_accelleration_on_GPU).

## Build Instructions

1. Install [CMake 3.25.1+](https://cmake.org/download/)
2. Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with MSVC v143 C++ Compiler
3. Install [Qt 6.7.3 MSVC2022 64bit](https://www.qt.io/download)
4. Set environment variable `Qt6_DIR` to your Qt path (e.g. `C:\Qt\6.7.3\msvc2022_64`)
5. Clone the repository:

    ```sh
    git clone https://github.com/berkbavas/ParticleSimulator.git
    ```

6. Create and enter a build directory:

    ```sh
    mkdir build
    cd build
    ```

7. Run CMake:

    ```sh
    cmake ..
    ```

8. Open `ParticleSimulator.sln` in Visual Studio and build.

## Demo

[Demo Video](https://github.com/user-attachments/assets/415fceb0-d4ac-46ca-ad2a-748fa7e5bdc5)

## License

This project is licensed under the [MIT License](LICENSE).
