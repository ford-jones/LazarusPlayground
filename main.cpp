/* =========================================
    Title: "Lazarus: Demo World"
    Author: Ford Jones
    Updated: Dec 21 2024
    Engine: Lazarus v0.4.0

    Tested Platforms:
    Linux Ubuntu 22.04.5 LTS 
    - Arch: x86_64
    - CPU: Intel i7-10510U (8) @ 4.900GHz
    - GPU: Intel CometLake-U GT2 [UHD Graphics]
============================================ */

#include "game.h"

int main()
{
    Game game;
    
    game.init();
    game.start();

    return 0;
};