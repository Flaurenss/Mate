#pragma once
#include <string>

class Engine
{
public:
	Engine();
	~Engine();

	void Initialize();
	void Run();
	
private:
	std::string title;
	int widht;
	int height;
	bool isRunning;

	void ProcessInput();
	void Update();
	void Render();
};

