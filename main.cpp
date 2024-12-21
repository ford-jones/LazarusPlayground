/* =========================================
    Title: "Lazarus: Demo World"
    Author: Ford Jones
    Updated: Dec 21 2024
    Engine: Lazarus v0.4.0

    Tested Platforms:
    
    Asus Zenbook
    - OS: Linux Ubuntu 22.04.5 LTS 
    - Arch: x86_64
    - CPU: Intel i7-10510U (8) @ 4.900GHz
    - GPU: Intel CometLake-U GT2 [UHD Graphics]

    Mac Mini (2014)
    - OS: MacOS Monterey 12.6.3
    - Arch: x86_64
    - CPU: 1.4 GHz Dual-Core Intel Core i5
    - GPU: Intel HD Graphics 5000 1536 MB
============================================ */

#include "game.h"

int main()
{
    Game game;
    
    game.init();
    game.start();

    return 0;
};