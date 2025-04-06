#pragma once
#include <memory>

class ECS;

class Entity
{
public:
    Entity(int id, ECS* registry);
    ~Entity(); // Declaración

    // --- Métodos de Componentes (Delegan a ECS) ---
    template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template <typename TComponent> void RemoveComponent();
    template <typename TComponent> bool HasComponent() const;
    template <typename TComponent> TComponent& GetComponent() const;

    // --- Otros Métodos ---
    int GetId() const; // Declaración

    // --- Operadores ---
    bool operator==(const Entity& entity) const; // Declaración
    bool operator<(const Entity& entity) const;  // Declaración
    bool operator>(const Entity& entity) const;  // Declaración

    ECS* registry; // Puntero a la instancia ECS activa
private:
    int id;
};

#include "Entity.inl"