/*
Set Primary Monitor Main

Copyright (c) 2016 Kobchaipuk Kemapirom
*/

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "user32.lib")

HMONITOR primary_hmonitor = 0;
DEVMODE primary_devmode = {};
