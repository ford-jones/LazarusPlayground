# Lazarus Playground 
A test demo showcasing the features of Lazarus.

## Getting Started:
1. Clone down the [Lazarus repo](https://github.com/ford-jones/Lazarus)
2. Follow the [installation instructions](https://github.com/ford-jones/Lazarus/blob/main/docs/getting-started.md)
3. Compile the program. 

### g++ / gcc:
```
g++ main.cpp game.cpp -O3 -o run -lGL -lGLEW -lglfw -lfmod -lfreetype -llazarus
```
### clang++ / clang:
```
clang++ -std=c++17 main.cpp game.cpp -o run -lGL -lGLEW -lglfw -lfmod -llazarus -lfreetype
```
### MSVC (Microsoft Visual Studio 2022):
```
cl /EHsc /std:c++17 /Zc:__cplusplus main.cpp game.cpp /link fmod_vc.lib freetype.lib glfw3.lib glew32.lib opengl32.lib liblazarus.lib msvcrt.lib user32.lib gdi32.lib shell32.lib /out:run.exe /NODEFAULTLIB:libcmt
```
4. Once compiled the game world can be launched like so:
```
./run
```

## Tested Platforms:
The engine should run on all major desktop environments. This program has also been tested on `arm64` MacOS (M1 and M2), although the GPU and other specs those machines ran are now unknown to me.

### Linux:
```
Asus Zenbook
- OS: Linux Ubuntu 22.04.5 LTS 
- Arch: x86_64
- CPU: Intel i7-10510U (8) @ 4.900GHz
- GPU: Intel CometLake-U GT2 [UHD Graphics]
```
### MacOS:
``` 
Mac Mini (2014)
- OS: MacOS Monterey 12.6.3
- Arch: x86_64
- CPU: 1.4 GHz Dual-Core Intel Core i5
- GPU: Intel HD Graphics 5000 1536 MB
```
### Windows:
```
MSI Prestige 15
- OS: Windows 10
- Arch: x86 AND x64
- CPU: Intel Core i7-10710U @ 1.10GHz
- GPU: Intel UHD Graphics
```