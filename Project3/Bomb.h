#pragma once
#include "Object.h"

class BaseScene;

class Bomb :
    public Object
{
public:
	Bomb(int& id, Vector2& pos,chronoTime& time, BaseScene& scene);
	~Bomb();

	bool Update(LayerVec&& layer)override;
	void Draw(void)override;

	bool UpdateDef(LayerVec& layer)override;						// 入力処理管理
	bool UpdateAuto(LayerVec& layer)override;						// オートパイロット
	bool UpdateRev(LayerVec& layer)override;						// 受信

private:
	BaseScene& scene_;

	chronoTime startTime_;

	int dethCnt_;
};

