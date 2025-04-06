#pragma once
#include <memory>

class ECS;

class Entity
{
public:
    Entity(int id, ECS* registry);
    ~Entity(); // Declaraci�n

    // --- M�todos de Componentes (Delegan a ECS) ---
    template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template <typename TComponent> void RemoveComponent();
    template <typename TComponent> bool HasComponent() const;
    template <typename TComponent> TComponent& GetComponent() const;

    // --- Otros M�todos ---
    int GetId() const; // Declaraci�n

    // --- Operadores ---
    bool operator==(const Entity& entity) const; // Declaraci�n
    bool operator<(const Entity& entity) const;  // Declaraci�n
    bool operator>(const Entity& entity) const;  // Declaraci�n

    ECS* registry; // Puntero a la instancia ECS activa
private:
    int id;
};

#include "Entity.inl"