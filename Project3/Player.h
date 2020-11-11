#pragma once
#include <map>
#include "common/Vector2.h"
#include "common/TileMap.h"
#include "DIR.h"
class Player
{
public:
	Player();
	Player(Vector2& pos);

	~Player();

	bool Update(LayerVec&& layer);
	void Draw(void);
	
	bool CheckWall(LayerVec& layer);

private:
	DIR dir_;

	Vector2 pos_;
	float rad_;
	Vector2 vel_;

	std::map<DIR, Vector2> dirMap_;			// ƒL[‚ªŒü‚«‚ÅˆÚ“®—Ê‚ª“ü‚é

	int animCnt_;
	int id_;
};

