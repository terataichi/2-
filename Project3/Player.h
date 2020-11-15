#pragma once
#include <map>
#include <functional>
#include "common/Vector2.h"
#include "common/TileMap.h"
#include "DIR.h"
#include "Object.h"

class Player:
	Object
{
public:
	Player();
	Player(int& id, Vector2& pos);

	~Player();

	bool Update(LayerVec&& layer);
	void Draw(void);
	
	bool CheckWall(LayerVec& layer);

	bool HostData(LayerVec& layer);
	bool GuestData(LayerVec& layer);
private:
	DIR dir_;


	std::map<DIR, Vector2> dirMap_;							// ƒL[‚ªŒü‚«‚ÅˆÚ“®—Ê‚ª“ü‚é


	std::function<bool(LayerVec&)>netFunc_;

	int animCnt_;
};

