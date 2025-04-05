#pragma once

class Entity;

struct IComponent 
{
protected:
	static int nextId;
};

/// <summary>
/// Pure data represented as a Component.
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class Component : public IComponent
{
public:
	static int GetId()
	{
		static auto id = nextId++;
		return id;
	}
private:
	Entity* owner;
};
