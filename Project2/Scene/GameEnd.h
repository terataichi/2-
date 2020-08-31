#pragma once
#include "BaseScene.h"
#include "SceneMng.h"

class GameEnd :
	public BaseScene
{
public:
	GameEnd();
	~GameEnd();

	uniqueBase Update(uniqueBase own) override;
	void Draw(void)override;
};

