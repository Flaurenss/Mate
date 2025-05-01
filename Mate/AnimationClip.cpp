#include "AnimationClip.h"
#include "Logger.h"

#include "ozz/base/memory/allocator.h" 

AnimationClip::AnimationClip(const std::string& name, std::shared_ptr<ozz::animation::Animation> animation) :
    name(name), animation(animation) {}

std::shared_ptr<AnimationClip> AnimationClip::BuildFromRawTracks(std::string name, std::vector<RawAnimationClip> tracks, float duration)
{
    ozz::animation::offline::RawAnimation raw;
    raw.duration = duration;
    raw.tracks.resize(tracks.size());

    for (auto i = 0; i < tracks.size(); i++)
    {
        const RawAnimationClip& src = tracks[i];
        auto& dst = raw.tracks[i];

        for (const auto& [time, position] : src.translations)
        {
            dst.translations.push_back({ time, ozz::math::Float3(position.x, position.y, position.z) });
        }
        for (const auto& [time, rotation] : src.rotations)
        {
            dst.rotations.push_back({ time, ozz::math::Normalize(ozz::math::Quaternion(rotation.x, rotation.y, rotation.z, rotation.w)) });
        }
        for (const auto& [time, scale] : src.scales)
        {
            dst.scales.push_back({ time, ozz::math::Float3(scale.x, scale.y, scale.z) });
        }
    }

    if (!raw.Validate())
    {
        Logger::Err("Invalid raw animation.");
        return nullptr;
    }

    ozz::animation::offline::AnimationBuilder builder;
    auto runtimeUnique = builder(raw);
    ozz::animation::Animation* rawPtr = runtimeUnique.get();
    
    auto deleter = runtimeUnique.get_deleter();
    auto shared = std::shared_ptr<ozz::animation::Animation>(rawPtr, deleter);
    runtimeUnique.release();
    
    return std::make_shared<AnimationClip>(name, shared);
}

const std::string& AnimationClip::GetName() const
{
    return name;
}

std::shared_ptr<ozz::animation::Animation> AnimationClip::GetAnimation() const
{
    return animation;
}
