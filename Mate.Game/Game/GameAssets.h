#pragma once

#include <Mate.h>

class GameAssets
{
public:
	static Entity CreatePlayer(MateEngine* engine, Vector3 basePos = Vector3(), Vector3 baseRot = Vector3(), Vector3 baseScale = Vector3::One);
	static Entity CreateReward(MateEngine* engine, Vector3 basePos = Vector3(), Vector3 baseRot = Vector3(), Vector3 baseScale = Vector3::One);
	static Entity CreateObstacle(MateEngine* engine, Vector3 basePos = Vector3(), Vector3 baseRot = Vector3(), Vector3 baseScale = Vector3::One);
};

