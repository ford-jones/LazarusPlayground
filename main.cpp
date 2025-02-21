/* =========================================
    Title: "Lazarus: Demo World"
    Author: Ford Jones
    Updated: Feb 20 2025
    Engine: Lazarus v0.7.0

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

    MSI Prestige 15
    - OS: Windows 10
    - Arch: x86
    - CPU: Intel Core i7-10710U @ 1.10GHz
    - GPU: Intel UHD Graphics
============================================ */

#include "game.h"

int main()
{
    Game game;
    
    game.init();
    game.start();

    return 0;
};