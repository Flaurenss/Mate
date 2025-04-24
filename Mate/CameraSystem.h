#pragma once
#include "ECS.h"
#include "Shader.h"

class CameraSystem : public System
{
public:
	CameraSystem(Shader& shader);
	
	virtual void Start();
	virtual void Update();
	virtual void End();

	void SetResolution(int w, int h);
private:
	int width;
	int height;
	Shader shader;
};