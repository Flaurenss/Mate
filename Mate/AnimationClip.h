#pragma once
#include <Vector.h>
#include <memory>
#include <vector>
#include <string>

#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/offline/raw_animation.h"
#include "ozz/animation/offline/animation_builder.h"

struct RawAnimationClip {
    std::string name;
    std::vector<std::pair<float, Vector3>> translations;
    std::vector<std::pair<float, Vector4>> rotations; // Quaternion for the moment
    std::vector<std::pair<float, Vector3>> scales;
};

class AnimationClip
{
public:
    AnimationClip(const std::string& name, std::shared_ptr<ozz::animation::Animation> animation);

    static std::shared_ptr<AnimationClip> BuildFromRawTracks(
        std::string name,
        std::vector<RawAnimationClip>,
        float duration);

    const std::string& GetName() const;
    std::shared_ptr<ozz::animation::Animation> GetAnimation() const;

private:
    std::string name;
    std::shared_ptr<ozz::animation::Animation> animation;
};

