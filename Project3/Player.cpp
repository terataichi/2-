#include "Player.h"
#include "common/ImageMng.h"
#include <DxLib.h>

Player::Player()
{
}

Player::Player(Vector2& pos)
{
	pos_ = pos;
	vel_ = { 5,5 };
	rad_ = 0;
}

Player::~Player()
{
}

bool Player::Update()
{
	return false;
}

void Player::Draw()
{
	auto handle = lpImageMng.GetHandle("Image/bomberman.png", { 5,4 }, { 32,51 });

	DrawRotaGraph(pos_.x + 16, pos_.y - 16, 1, 0, handle[0], true);
}
