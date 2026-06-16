/* =========================================
    Title: "Lazarus: Demo World"
    Author: Ford Jones
    Updated: Sep 8 2025
    Engine: Lazarus v0.12.6

    Tested Platforms:
    
    MSI GF65
    - OS: Windows 11
    - Arch: x86
    - CPU: Intel 10th Gen Core i5-10400H 4-Core 2.6 – 4.6GHz
    - GPU: NVIDIA GeForce GTX 1650

    MSI Prestige 15
    - OS: Windows 10
    - Arch: x86
    - CPU: Intel Core i7-10710U @ 1.10GHz
    - GPU: Intel UHD Graphics
    
    Dell Optiplex 9020                              (verify these specs)
    - OS: Windows 10
    - Arch: x86
    - CPU: Intel 4th Gen Core i7 3.4ghz
    - GPU: AMD Radeon RX 590
    
    Asus Zenbook
    - OS: Linux Debian 13.0 (Trixee)
    - Graphical session: KDE-plasma (X11)
    - Arch: x86_64
    - CPU: Intel i7-10510U (8) @ 4.900GHz
    - GPU: Intel CometLake-U GT2 [UHD Graphics]

    Asus Zenbook
    - OS: Linux Ubuntu 22.04.5 (Jammy)
    - Graphical session: Gnome (wayland)
    - Arch: x86_64
    - CPU: Intel i7-10510U (8) @ 4.900GHz
    - GPU: Intel CometLake-U GT2 [UHD Graphics]

    Mac Mini (2014)
    - OS: MacOS Monterey 12.6.3
    - Arch: x86_64
    - CPU: 1.4 GHz Dual-Core Intel Core i5
    - GPU: Intel HD Graphics 5000 1536 MB

    MacBook Pro (2020)
    - OS: Sonoma 14.8.7
    - Arch: Arm64
    - CPU: Apple M1
    - GPU: M1 SoC (System-on-Chip, unified graphics)
============================================ */

#include "game.h"

int main()
{
    Game game;
    
    game.init();
    game.start();

    return 0;
};