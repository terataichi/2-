#pragma once
#include <vector>
#include "common/Vector2.h"


/// <summary>
/// 炎の爆風生成クラス
/// </summary>
class FlameGenerator
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="length">爆発の幅</param>
	/// <param name="pos">爆発する中心の座標</param>
	FlameGenerator(int length, Vector2& pos);
	~FlameGenerator();

	bool Update(void);

private:

	int length_;
	Vector2 pos_;

	int time_;
	int count_;
};

