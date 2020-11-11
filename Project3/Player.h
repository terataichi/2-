#pragma once
#include "common/Vector2.h"

class Player
{
public:
	Player();
	Player(Vector2& pos);

	~Player();

	bool Update();
	void Draw();

private:
	Vector2 pos_;
	float rad_;
	Vector2 vel_;

	int myID_;
};

