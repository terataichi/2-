#include "Player.h"
#include <DxLib.h>
#include <chrono>
#include <cassert>
#include "common/ImageMng.h"
#include "_debug/_DebugConOut.h"
#include "NetWork/NetWork.h"
#include "Scene/GameScene.h"
#include "Input/PadState.h"
#include "Input/KeyState.h"

int Player::lostCont_ = 0;

Player::Player(Vector2& pos, BaseScene& scene) :scene_(scene)
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

	input_ = std::make_unique<KeyState>();

	input_->SetUp(0);

	int modeID = lpNetWork.GetNetWorkMode() == NetWorkMode::HOST ? 1 : 0;

	// ボムリストの初期化
	for (int id = id_ + 1; id < id_ + UNIT_ID_BASE; id++)
	{
		bombList_.emplace_back(id);
	}

	if (lpNetWork.GetNetWorkMode() == NetWorkMode::OFFLINE)
	{
		if (id_ == 0)
		{
			netFunc_ = std::bind(&Player::UpdateDef, this, std::placeholders::_1);
			wallFunc_ = std::bind(&Player::WallInput, this, std::placeholders::_1);
			return;
		}
		netFunc_ = std::bind(&Player::UpdateAuto, this, std::placeholders::_1);
		wallFunc_ = std::bind(&Player::WallAuto, this, std::placeholders::_1);
	}
	else
	{
		if (id_ / UNIT_ID_BASE == modeID)
		{
			netFunc_ = std::bind(&Player::UpdateDef, this, std::placeholders::_1);
			wallFunc_ = std::bind(&Player::WallInput, this, std::placeholders::_1);
		}
		else
		{
			if (id_ != modeID)
			{

				netFunc_ = std::bind(&Player::UpdateAuto, this, std::placeholders::_1);
				wallFunc_ = std::bind(&Player::WallAuto, this, std::placeholders::_1);
			}
			else
			{
				netFunc_ = std::bind(&Player::UpdateRev, this, std::placeholders::_1);
			}
		}
	}


}

Player::~Player()
{
}

bool Player::Update(LayerVec&& vecLayer)
{
	// ID別処理
	return 	netFunc_(vecLayer);
}

void Player::Draw()
{
	VecInt handle = lpImageMng.GetHandle("Image/bomberman.png", { 5,4 }, { 32,51 });

	auto tmpCnt = (animCnt_ / 10);
	DrawGraph(pos_.x, pos_.y - 30, handle[((tmpCnt % 4) * 5) + static_cast<int>(dir_)], true);
	animCnt_++;
}

bool Player::CheckWall(LayerVec& vecLayer)
{
	if (pos_.x % 32 == 0)
	{
		if (pos_.y % 32 == 0)
		{
			for (auto& layer : vecLayer)
			{
				if (layer.name == "Obj")
				{
					while (!wallFunc_(layer))
					{
					}
					break;
				}

			}
		}
	}

	//TRACE("%d\n", static_cast<int>(dir_));
	return true;
}

bool Player::UpdateDef(LayerVec& layer)
{
	// 入力のアップデート
	(*input_)();

	CheckWall(layer);

	auto move = [&](INPUT_ID id, Vector2 vel)
	{
		if (input_->GetTrgPush(id))
		{
			// ここに壁判定追加予定
			pos_ += vel;
		}
	};

	move(INPUT_ID::BUTTON_DOWN, dirMap_[DIR::DOWN] * vel_);
	move(INPUT_ID::BUTTON_LEFT, dirMap_[DIR::LEFT] * vel_);
	move(INPUT_ID::BUTTON_RIGHT, dirMap_[DIR::RIGHT] * vel_);
	move(INPUT_ID::BUTTON_UP, dirMap_[DIR::UP] * vel_);

	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_ATTACK))
	{
		auto no = CheckBomb();
		if (no >= 0)
		{
			try
			{
				chronoTime time = std::chrono::system_clock::now();
				dynamic_cast<GameScene&>(scene_).SetBomb(id_, no, pos_,time, true);
			}
			catch (...)
			{
				assert(!"シーンのキャスト失敗");
			}
		}
	}


	UnionData data[4]{};
	data[0].iData = id_;
	data[1].iData = pos_.x;
	data[2].iData = pos_.y;
	data[3].iData = static_cast<int>(dir_);

	lpNetWork.SendMes(MesType::POS, UnionVec{ data[0],data[1],data[2] ,data[3] });

	return true;
}

bool Player::UpdateAuto(LayerVec& layer)
{
	CheckWall(layer);
	pos_ += (dirMap_[dir_] * vel_);

	UnionData data[4]{};
	data[0].iData = id_;
	data[1].iData = pos_.x;
	data[2].iData = pos_.y;
	data[3].iData = static_cast<int>(dir_);

	lpNetWork.SendMes(MesType::POS, UnionVec{ data[0],data[1],data[2] ,data[3] });

	return true;
}

bool Player::UpdateRev(LayerVec& layer)
{
	bool count = false;
	while (CheckData(MesType::POS))
	{
		UnionVec data{};
		PickData(MesType::POS, data);

		if (data.size())
		{
			pos_.x = data[1].iData;
			pos_.y = data[2].iData;
			dir_ = static_cast<DIR>(data[3].iData);
		}
		count = true;     
	}
	
	while (CheckData(MesType::SET_BOMB))
	{
		UnionVec data{};
		PickData(MesType::SET_BOMB, data);

		if (data.size())
		{
			Vector2 pos{ data[2].iData,data[3].iData };
			try
			{
				TimeData timeData{};
				timeData.iData[0] = data[4].iData;
				timeData.iData[1] = data[5].iData;

				dynamic_cast<GameScene&>(scene_).SetBomb(data[0].iData, data[1].iData, pos,timeData.time ,false);
			}
			catch (...)
			{
				int a = 0;
			}
		}
		count = true;
	}

	if (!count)
	{
		lostCont_++;
		TRACE("取りこぼしID:%d\n", id_);
	}

	return true;
}

void Player::AddBombList(int no)
{
	bombList_.emplace_back(no);
}

int Player::CheckBomb()
{
	int id = -1;
	if (bombList_.size())
	{
		id = bombList_.front();
		bombList_.erase(bombList_.begin());
	}
	return id;
}

bool Player::WallAuto(LayerData& layer)
{
	Vector2 size{ (pos_.x / 32),(pos_.y / 32) };
	size += dirMap_[dir_];
	int num = ((size.x) + (size.y) * layer.width);

	if (layer.chipData[num] != 0)
	{
		++dir_;
		if (dir_ == end(dir_))
		{
			dir_ = begin(dir_);
		}
		return false;
	}
	return true;
}

bool Player::WallInput(LayerData& layer)
{
	return true;
}
