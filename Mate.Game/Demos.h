#pragma once

#include "./Game/EnvironmentAssets.h"
#include "CameraUtils.h"

class Demos : public IGame
{
public:
	Demos(MateEngine& engine);
	~Demos() = default;
	void Start() override;
	void Update(float deltaTime) override;
	void FixedUpdate(float fixedDeltaTime) override;
	bool IsGameRunning() const override;

private:
	MateEngine& engine;
	AnimationComponent& animator;
	std::unique_ptr<Entity> player;
	CameraUtils cameraUtils;
	bool isGameRunning = true;

	void DemoAssets();
	void DemoAnimations();

	Entity CreateCamera();
	AnimationComponent& GetPlayerAnimator();
};

