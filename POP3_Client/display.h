#ifndef DISPLAY_H
#define DISPLAY_H
#pragma once

// #include <dirent.h>
#include <Windows.h>
#include <iostream>

class Display
{
public:
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD COLOR_DIRECT = 2;  // GREEN
    WORD COLOR_ERROR = 4;   // RED
    WORD COLOR_DEFAULT = 7; // WHITE
    WORD COLOR_INFO = 1;    // BLUE
    Display()
    {
    }
    void home_screen();
};
#endif // DISPLAY_H
