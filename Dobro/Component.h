#pragma once

class Entity;

// Pure data
// Contiguous in memory, organized the data itself, not by entity (Data oriented design)
class Component
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
private:
	Entity* owner;
};
