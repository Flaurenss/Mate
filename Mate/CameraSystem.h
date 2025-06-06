#pragma once
#include "ECS.h"
#include <optional>
#include "RenderContext.h"

class CameraSystem : public System
{
public:
	CameraSystem();
	
	void Update(RenderContext& renderContext);
};