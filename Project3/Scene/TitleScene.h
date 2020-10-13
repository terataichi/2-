#pragma once
#include "BaseScene.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Init(void) override;
	uniqueBase Update(uniqueBase scene) override;
	void Draw() override;

private:
	int screen_size_x_;
	int screen_size_y_;
};

