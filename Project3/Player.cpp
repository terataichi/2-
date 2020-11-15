#include "Player.h"
#include "common/ImageMng.h"
#include "_debug/_DebugConOut.h"
#include "NetWork/NetWork.h"
#include <DxLib.h>

Player::Player()
{
	pos_ = {0,0};
	vel_ = { 4,4 };
	rad_ = 0;
	dir_ = DIR::DOWN;
	animCnt_ = 0;
}

Player::Player(int& id,Vector2& pos)
{
	pos_ = pos;
	vel_ = { 4,4 };
	rad_ = 0;
	dir_ = DIR::DOWN;
	animCnt_ = 0;

	dirMap_.try_emplace(DIR::DOWN, Vector2{ 0,1 });
	dirMap_.try_emplace(DIR::LEFT, Vector2{ -1,0 });
	dirMap_.try_emplace(DIR::RIGHT, Vector2{ 1,0 });
	dirMap_.try_emplace(DIR::UP, Vector2{ 0,-1 });

	int modeID = lpNetWork.GetNetWorkMode() == NetWorkMode::HOST;

	
	if (id % 2 == modeID)
	{
		netFunc_ = std::bind(&Player::HostData, this, std::placeholders::_1);
	}
	else
	{
		netFunc_ = std::bind(&Player::GuestData, this, std::placeholders::_1);
	}

}

Player::~Player()
{
}

bool Player::Update(LayerVec&& vecLayer)
{
	// IDï èàóù
	netFunc_(vecLayer);


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

					if (layer.chipData[num] != 7)
					{
						return true;
					}
					++dir_;
					if (dir_ == end(dir_))
					{
						dir_ = begin(dir_);
					}
				}
			}

		}
	}

	//TRACE("%d\n", static_cast<int>(dir_));
	return true;
}

bool Player::HostData(LayerVec& layer)
{
	CheckWall(layer);

	pos_ += (dirMap_[dir_] * vel_);

	UnionData data[3];
	data[0].iData = id_;
	data[1].iData = pos_.x;
	data[2].iData = pos_.y;

	lpNetWork.SendMes(MesType::POS, UnionVec{ data[0],data[1],data[2] });

	return true;
}

bool Player::GuestData(LayerVec& layer)
{

	while (lpNetWork.CheckMes(MesType::POS,id_))
	{
		UnionVec data;
		lpNetWork.PickRevData(MesType::POS, id_, data);

		pos_.x = data[1].iData;
		pos_.y = data[2].iData;
	}
	return true;
}
