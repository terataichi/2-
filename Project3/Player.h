#pragma once
#include <map>
#include <functional>
#include "common/Vector2.h"
#include "common/TileMap.h"
#include "DIR.h"
class Player
{
public:
	Player();
	Player(int id, Vector2& pos);

	~Player();

	bool Update(LayerVec&& layer);
	void Draw(void);
	
	bool CheckWall(LayerVec& layer);

	bool HostData(void);
	bool GuestData(void);
private:
	DIR dir_;

	Vector2 pos_;
	float rad_;
	Vector2 vel_;

	std::map<DIR, Vector2> dirMap_;							// ƒL[‚ªŒü‚«‚ÅˆÚ“®—Ê‚ª“ü‚é


	std::map<int, std::function<bool(void)>>netFunc_;

	int animCnt_;
	const int id_;
};

