#pragma once
#include <string>
#include "ECS.h"

class Engine
{
public:
	Engine(int width = 1920, int height = 1080);
	~Engine();

	void Initialize();
	void Run();
	
private:
	std::string title;
	int widht;
	int height;
	bool isRunning;

	std::unique_ptr<ECS> ecs;

	void ProcessInput();
	void Update();
	void Render();
};

