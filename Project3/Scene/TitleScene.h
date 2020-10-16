#pragma once
#include <memory>
#include <map>
#include <functional>
#include "BaseScene.h"
#include "../common/Vector2.h"
#include "../Input/InputState.h"
#include "../NetWork/NetWorkState.h"

enum class UpdateMode
{
	SetNetWork,
	SetHostIP,
	StartInit,
	Play,
};

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
	void PlayUpdate(void);
	void SetNetWorkMode(void);
	void SetHostIP(void);
	void StartInit(void);
	std::map<UpdateMode, std::function<void(void)>> func_;

	int screen_size_x_;
	int screen_size_y_;

	UpdateMode updateMode_;


	// player
	Vector2 pos_;
	int speed_;
	float rad_;

	std::unique_ptr<InputState> input_;
};

