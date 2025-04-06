#pragma once

// �Importante! Este archivo DEBE ser incluido al FINAL de Entity.h
// Incluye las cabeceras necesarias para las definiciones

#include "Entity.h" // Necesita la declaraci�n de Entity
#include <utility>  // Para std::forward
#include "ECS.h"    // Necesita la definici�n COMPLETA de ECS para llamar a sus m�todos

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args)
{
}

template <typename TComponent>
void Entity::RemoveComponent()
{
	registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const
{

}

template <typename TComponent>
TComponent& Entity::GetComponent() const
{

}

// Aseg�rate de que Entity::registry se inicializa en Entity.cpp
// Aseg�rate de llamar a Entity::SetRegistry() en tu c�digo principal