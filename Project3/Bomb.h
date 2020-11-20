#pragma once
#include "Object.h"

class BaseScene;

class Bomb :
    public Object
{
public:
	Bomb(int& id, Vector2& pos,chronoTime& time, BaseScene& scene);
	~Bomb();

	bool Update()override;
	void Draw(void)override;

	bool UpdateDef()override;						// 入力処理管理
	bool UpdateAuto()override;						// オートパイロット
	bool UpdateRev()override;						// 受信

private:
	BaseScene& scene_;

	chronoTime startTime_;

	int dethCnt_;
};

