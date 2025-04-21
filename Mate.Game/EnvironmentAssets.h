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
    EnvironmentType environmentType;
};

struct EnvironmentPart
{
    Part floorPart;
    std::vector<Part> collisions;
    std::vector<Part> rewards;
};
struct EnvironmentAsset
{
    EnvironmentPart floor;
};

struct PlayerRailState
{
    float targetX = 0.0f;
    int currentRail = 0;
};

