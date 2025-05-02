#pragma once
#include <string>
#include <unordered_map>
#include "Matrix.h"

class AnimationComponent
{
public:
    std::string ModelId;
    int CurrentAnimationIndex = 0;
    float PlaybackTime = 0.0f;

    void ClearCache();

    std::unordered_map<std::string, Matrix4>& GetCache();

private:
    std::unordered_map<std::string, Matrix4> jointTransforms;
};

