#include "OzzLoader.h"
#include "ozz/base/io/archive.h"
#include "ozz/base/io/stream.h"
#include "ozz/base/log.h"
#include "Logger.h"

std::unique_ptr<ozz::animation::Skeleton> OzzLoader::LoadSkeleton(const std::string& path)
{
    ozz::io::File file(path.c_str(), "rb");
    if (!file.opened())
    {
        Logger::Err("Failed to open skeleton file: " + path);
        return nullptr;
    }

    ozz::io::IArchive archive(&file);
    if (!archive.TestTag<ozz::animation::Skeleton>())
    {
        Logger::Err("Invalid skeleton archive: " + path);
        return nullptr;
    }

    auto skeleton = std::make_unique<ozz::animation::Skeleton>();
    archive >> *skeleton;

    return skeleton;
}

std::unique_ptr<ozz::animation::Animation> OzzLoader::LoadAnimation(const std::string& path)
{
    ozz::io::File file(path.c_str(), "rb");
    if (!file.opened())
    {
        Logger::Err("Failed to open animation file: " + path);
        return nullptr;
    }

    ozz::io::IArchive archive(&file);
    if (!archive.TestTag<ozz::animation::Animation>())
    {
        Logger::Err("Invalid animation archive: " + path);
        return nullptr;
    }

    auto animation = std::make_unique<ozz::animation::Animation>();
    archive >> *animation;

    return animation;
}
