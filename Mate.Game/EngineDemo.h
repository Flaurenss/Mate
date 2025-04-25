#pragma once
#include <ECS.h>
#include "ModelImporter.h"

class EngineDemo
{
public:
	static void CreateBaseFloor(ECS& ecs);
	static void PhysicsCubeDemo(ECS& ecs, ModelImporter& modelImporter);
};