#pragma once
#include <Mate.h>

enum EnvironmentType
{
    Floor,
    Obstacle,
    Reward
};

struct EnvironmentPart
{
    EnvironmentType environmentType;
    Entity entity;
    Vector3 originalPos;
};

struct EnvironmentAsset
{
    std::vector<EnvironmentPart> objects;
};

