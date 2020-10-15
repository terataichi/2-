#pragma once
#include <memory>
#include "BaseScene.h"
#include "../common/Vector2.h"
#include "../Input/InputState.h"


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

	Vector2 pos_;
	int speed_;
	float rad_;

	std::unique_ptr<InputState> input_;
};

