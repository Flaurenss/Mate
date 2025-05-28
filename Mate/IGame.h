#pragma once

class IGame
{
public:
	/// <summary>
	/// Executed only once after the Engine setup.
	/// </summary>
	virtual void Start() = 0;

	/// <summary>
	/// Executed every frame.
	/// </summary>
	/// <param name="deltaTime">The delta time from last frame.</param>
	virtual void Update(float deltaTime) = 0;

	/// <summary>
	/// Executed every physics update.
	/// </summary>
	/// <param name="fixedDeltaTime">The fixed delta time between physics updates. It is fixed.</param>
	virtual void FixedUpdate(float fixedDeltaTime) = 0;

	/// <summary>
	/// Sets the actual game status so the engine knows when to finish. 
	/// </summary>
	/// <returns>If the game is running.</returns>
	virtual bool IsGameRunning() const = 0;
	
	virtual ~IGame() = default;
};