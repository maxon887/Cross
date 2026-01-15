#pragma once
#include <Windows.h>
#undef CreateDirectory

LRESULT CALLBACK WinProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND WinCreate();