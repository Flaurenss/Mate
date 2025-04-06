#pragma once
#include <string>
#include "ECS.h"

class Engine
{
public:
	Engine(int width = 1920, int height = 1080);
	~Engine();

	void Initialize();
	bool IsRunning();
	void ProcessInput();
	void Update();
	void Render();

private:
	std::string title;
	int widht;
	int height;
	bool isRunning;
	void Run();

	std::unique_ptr<ECS> registry;
};

