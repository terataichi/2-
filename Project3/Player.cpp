#include "Player.h"
#include "common/ImageMng.h"
#include "_debug/_DebugConOut.h"
#include "NetWork/NetWork.h"
#include <DxLib.h>

Player::Player():id_(0)
{
	pos_ = {0,0};
	vel_ = { 4,4 };
	rad_ = 0;
	dir_ = DIR::DOWN;
	animCnt_ = 0;
}

Player::Player(int id,Vector2& pos):id_(id),pos_(pos)
{
	vel_ = { 4,4 };
	rad_ = 0;
	dir_ = DIR::DOWN;
	animCnt_ = 0;

	dirMap_.try_emplace(DIR::DOWN, Vector2{ 0,1 });
	dirMap_.try_emplace(DIR::LEFT, Vector2{ -1,0 });
	dirMap_.try_emplace(DIR::RIGHT, Vector2{ 1,0 });
	dirMap_.try_emplace(DIR::UP, Vector2{ 0,-1 });

	//netFunc_.try_emplace(0,std::bind(&HostSend));
	//netFunc_.try_emplace(1);

}

Player::~Player()
{
}

bool Player::Update(LayerVec&& vecLayer)
{

	//if (id_ % 2 == 0)
	//{
	CheckWall(vecLayer);
	//}
	pos_ += (dirMap_[dir_] * vel_);

	UnionVec vecData;
	UnionData dirData;
	dirData.iData = pos_.x;
	vecData.emplace_back(dirData);

	dirData.iData = pos_.y;
	vecData.emplace_back(dirData);

	lpNetWork.SendMes(MesType::POS, vecData);

	return true;
}

void Player::Draw()
{
	auto handle = lpImageMng.GetHandle("Image/bomberman.png", { 5,4 }, { 32,51 });


	auto tmpCnt = (animCnt_ / 10);
	DrawGraph(pos_.x, pos_.y - 30, handle[((tmpCnt % 4) * 5) + static_cast<int>(dir_)], true);
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
						//UnionData dirData;
						//dirData.iData = static_cast<int>(dir_);

						//UnionVec vecData;
						//vecData.emplace_back(dirData);

						//lpNetWork.SendMes(MesType::POS, vecData);
						return true;
					}
				}
			}

		}
	}

	//TRACE("%d\n", static_cast<int>(dir_));
	return true;
}

bool Player::HostData(void)
{
	return false;
}

bool Player::GuestData(void)
{
	return false;
}
