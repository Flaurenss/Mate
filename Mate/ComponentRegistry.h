#pragma once
#include <vector>

class IRegistry
{
public:
	virtual ~IRegistry() {}
};

template <typename TComponent>
class ComponentRegistry : public IRegistry
{
public:
	ComponentRegistry(int size = 100);
	virtual ~ComponentRegistry() = default;

	bool IsEmpty() const;
	int GetSize() const;

	void Resize(int size);
	void Clear();
	void Add(TComponent component);
	void Set(int index, TComponent component);
	TComponent& Get(int index);
	TComponent& operator [](int index);
private:
	std::vector<TComponent> data;
};
