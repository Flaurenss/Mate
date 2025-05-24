#pragma once

class IGame
{
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void FixedUpdate(float fixedDeltaTime) = 0;
	virtual bool IsGameRunning() const = 0;
	virtual ~IGame() = default;
};