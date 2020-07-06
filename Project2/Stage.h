#pragma once
#include "common/Vector2.h"

class Stage
{
public:
	Stage();
	Stage(Vector2 pos);
	~Stage();

	const int GetStageID(void)const;					// ステージID書き込み用
	void Draw(void);									// 各スクリーンに描画する
private:
	void Init(Vector2 pos);

	Vector2 _pos;
	int _StageID;										// ステージの描画用スクリーンID
};

