#include "Player.h"
#include "common/ImageMng.h"
#include "_debug/_DebugConOut.h"
#include <DxLib.h>

Player::Player()
{
	pos_ = {0,0};
	vel_ = { 4,4 };
	rad_ = 0;
	dir_ = DIR::DOWN;
	animCnt_ = 0;
}

Player::Player(Vector2& pos)
{
	pos_ = pos;
	vel_ = { 1,1 };
	rad_ = 0;
	dir_ = DIR::DOWN;
	animCnt_ = 0;

	dirMap_.try_emplace(DIR::DOWN, Vector2{ 0,vel_.y });
	dirMap_.try_emplace(DIR::LEFT, Vector2{ -vel_.x,0 });
	dirMap_.try_emplace(DIR::RIGHT, Vector2{ vel_.x,0 });
	dirMap_.try_emplace(DIR::UP, Vector2{ 0,-vel_.y });

}

Player::~Player()
{
}

bool Player::Update(LayerVec&& vecLayer)
{
	auto move = [&](DIR dir, Vector2 pos)
	{
		if (dir == dir_)
		{
			pos_ += pos;
		}
	};

	//move(DIR::DOWN, { 0,1 });
	//move(DIR::LEFT, { -1,0 });
	//move(DIR::RIGHT, { 1,0 });
	//move(DIR::UP, { 0,-1 });

	CheckWall(vecLayer);
	pos_ += dirMap_[dir_];

	return true;
}

void Player::Draw()
{
	auto handle = lpImageMng.GetHandle("Image/bomberman.png", { 5,4 }, { 32,32 });


	auto tmpCnt = (animCnt_ / 30);
	DrawGraph(pos_.x, pos_.y,handle[((tmpCnt % 4) * 5) + static_cast<int>(dir_) ], true);
	animCnt_++;
}

bool Player::CheckWall(LayerVec& vecLayer)
{
	if (pos_.x % 32 == 0 && pos_.y % 32 == 0)
	{
		for (auto layer : vecLayer)
		{
			if (layer.name == "Obj")
			{
				while (true)
				{
					Vector2 size{ (pos_.x / 32),(pos_.y / 32) };
					size += dirMap_[dir_];
					int num = ((size.x) + (size.y) * layer.width);

					if (layer.chipData[num] == 7)
					{
						++dir_;
						if (dir_ == end(dir_))
						{
							dir_ = begin(dir_);
						}
					}
					else
					{
						break;
					}
				}
			}

		}
	}
	TRACE("%d\n", static_cast<int>(dir_));
	return true;
}
