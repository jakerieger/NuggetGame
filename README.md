# Nugget Game

**Nugget Game** is a 2D platform physics puzzle game for Windows and Linux. Solve the puzzles to reach the sauce at
the end of each level.

## Structure

This repository contains the following subdirectories:

- [Assets](Assets) - Source game assets such as sprites, audio, levels, etc...
- [Core](Core) - Game engine library source
- [Game](Game) - Nugget Game source
- [Lib](Lib) - Third party libraries
- [Packer](Packer) - Asset packing and unpacking source

## Building

The general process for compiling and running Nugget Game is as follows:

1. Build `Core` and `Packer` static libraries
2. Copy `Assets` directory to build directory
3. Build and run `NugPak` executable to pack game assets
4. Build and run `NuggetGame` executable to launch the game

The only files and folders actually needed to run the game after the above steps
have been completed are:

- `Data/`
- `glfw3.dll`
- `lz4.dll` (`lz4d.dll` for debug)
- `NuggetGame.exe`

## Dependencies

Nugget Game relies on the following third party libraries to compile successfully:

- [GLFW3](https://www.glfw.org/)
- [box2d](https://box2d.org/)
- [LZ4](https://github.com/lz4/lz4)
- [glm](https://github.com/g-truc/glm)
- [glad](https://glad.dav1d.de/)
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)