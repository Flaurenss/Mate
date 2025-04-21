#pragma once
#include <Mate.h>
#include <vector>

enum EnvironmentType
{
    Floor,
    Obstacle,
    Reward
};

struct Part
{
    Entity entity;
    Vector3 originalPos;
    TransformComponent& transformComponent;
    EnvironmentType environmentType;
};

struct EnvironmentPart
{
    Part floorPart;
    std::vector<Part> collisions;
    std::vector<Part> rewards;

    TransformComponent& GetFloorTransform()
    {
        return floorPart.transformComponent;
    }
};

struct PlayerRailState
{
    float targetX = 0.0f;
    int currentRail = 0;
};

