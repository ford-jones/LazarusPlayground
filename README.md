# Lazarus Playground 
A test demo showcasing the features of Lazarus.

## Getting Started:
1. Clone down the [Lazarus repo](https://github.com/ford-jones/Lazarus)
2. Follow the [installation instructions](https://github.com/ford-jones/Lazarus/blob/main/docs/getting-started.md)
3. Compile the program:
```
g++ main.cpp game.cpp -o -lGL -lGLEW -lglfw -lfmod -lfreetype -llazarus
```
or
```
clang++ -std=c++17 main.cpp game.cpp -lGL -lGLEW -lglfw -lfmod -llazarus -lfreetype
```
4. Once compiled the game world can be launched like so:
```
./a.out
```
