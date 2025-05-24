#pragma once
#include <Matrix.h>

struct RenderContext
{
	Matrix4 Projection;
	Matrix4 View;
	int Width;
	int Height;
	bool DebugMode;
};