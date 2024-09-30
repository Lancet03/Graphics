#pragma once
#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

struct PenParams {
	int style, width;
	COLORREF color;
};

struct GdiplusPenParams {
	int width;
	Gdiplus::Color color;
};

