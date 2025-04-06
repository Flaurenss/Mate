#pragma once
#include <memory>

class ECS;

class Entity
{
public:
    Entity(int id, ECS* registry);
    void Destroy();

    template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template <typename TComponent> void RemoveComponent();
    template <typename TComponent> bool HasComponent() const;
    template <typename TComponent> TComponent& GetComponent() const;

    int GetId() const;

    bool operator==(const Entity& entity) const;
    bool operator<(const Entity& entity) const;
    bool operator>(const Entity& entity) const;

    ECS* registry;
private:
    int id;
};

#include "Entity.inl"