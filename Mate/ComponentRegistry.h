#pragma once
#include <vector>
#include <memory>

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

	void Clear();
	void Add(TComponent component);
	void Set(int index, TComponent component);
	void Remove(int ientityId);
	TComponent& Get(int index);
	TComponent& operator [](int index);
private:
	std::vector<std::shared_ptr<TComponent>> data;
};

template<typename TComponent>
ComponentRegistry<TComponent>::ComponentRegistry(int size)
{
	data.resize(size, nullptr);
}

template<typename TComponent>
bool ComponentRegistry<TComponent>::IsEmpty() const
{
	return data.empty();
}

template<typename TComponent>
int ComponentRegistry<TComponent>::GetSize() const
{
	return data.size();
}

template<typename TComponent>
void ComponentRegistry<TComponent>::Clear()
{
	data.clear();
}

template<typename TComponent>
void ComponentRegistry<TComponent>::Add(TComponent component)
{
	data.push_back(component);
}

template<typename TComponent>
void ComponentRegistry<TComponent>::Set(int index, TComponent component)
{
	if (index >= data.size())
	{
		data.resize(index + 1, nullptr);
	}
	data[index] = std::make_shared<TComponent>(component);
}

template<typename TComponent>
inline void ComponentRegistry<TComponent>::Remove(int entityId)
{
	// TODO:... Delete entity
}

template<typename TComponent>
TComponent& ComponentRegistry<TComponent>::Get(int index)
{
	// TODO: check if nullptr
	return *(data[index]);
}

template<typename TComponent>
TComponent& ComponentRegistry<TComponent>::operator[](int index)
{
	return *data[index];
}