#pragma once

class Engine
{
public:
	Engine();
	~Engine();

	void Initialize();
	void Run();
	void ProcessInput();
	void Update();
private:
	int widht;
	int height;
};

