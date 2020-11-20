#include "Player.h"
#include <DxLib.h>
#include <chrono>
#include <cassert>
#include "common/ImageMng.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "NetWork/NetWork.h"
#include "Scene/GameScene.h"
#include "Input/PadState.h"
#include "Input/KeyState.h"

int Player::lostCont_ = 0;

Player::Player(Vector2& pos, BaseScene& scene,LayerVec& layer):scene_(scene),layerData_(layer)
{
	pos_ = pos;
	vel_ = { 4,4 };
	rad_ = 0;
	dir_ = DIR::DOWN;
	animCnt_ = 0;
	dirCnt_ = 0;

	dirMap_.try_emplace(DIR::DOWN, Vector2{ 0,1 });
	dirMap_.try_emplace(DIR::LEFT, Vector2{ -1,0 });
	dirMap_.try_emplace(DIR::RIGHT, Vector2{ 1,0 });
	dirMap_.try_emplace(DIR::UP, Vector2{ 0,-1 });

	input_ = std::make_unique<KeyState>();

	input_->SetUp(0);

	nextDir_[0] = DIR::DOWN;
	nextDir_[1] = DIR::LEFT;
	nextDir_[2] = DIR::UP;
	nextDir_[3] = DIR::RIGHT;
	nextDir_[4] = DIR::MAX;

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
			netFunc_ = std::bind(&Player::UpdateDef, this);
			InitFunc();

			return;
		}
		netFunc_ = std::bind(&Player::UpdateAuto, this);
		state_ = STATE::Run;
	}
	else
	{
		if (id_ / UNIT_ID_BASE == modeID)
		{
			netFunc_ = std::bind(&Player::UpdateDef,this);
		}
		else
		{
			if (id_ != modeID)
			{

				netFunc_ = std::bind(&Player::UpdateAuto, this);
				state_ = STATE::Run;
			}
			else
			{
				netFunc_ = std::bind(&Player::UpdateRev, this);
			}
		}
	}

}

Player::~Player()
{
}

bool Player::Update()
{
	// ID別処理
	return 	netFunc_();
}

void Player::Draw()
{
	VecInt handle = lpImageMng.GetHandle("Image/bomberman.png", { 5,4 }, { 32,51 });

	auto tmpCnt = (animCnt_ / 10);

	int animNun = 0;
	if (state_ == STATE::Run)
	{
		animNun = 2;
	}

	DrawGraph(pos_.x, pos_.y - 30, handle[((animNun +(tmpCnt % 2)) * 5) + static_cast<int>(dir_)], true);

	DrawBox(pos_.x, pos_.y, pos_.x + 32, pos_.y + 32, 0xffff, false);
	animCnt_++;
}

bool Player::CheckWallAuto()
{
	if (pos_.x % 32 == 0)
	{
		if (pos_.y % 32 == 0)
		{
			for (auto& layer : layerData_)
			{
				if (layer.name == "Obj")
				{
					while (true)
					{
						Vector2 size{ (pos_.x / 32),(pos_.y / 32) };
						size += dirMap_[dir_];
						int num = ((size.x) + (size.y) * layer.width);

						if (layer.chipData[num] != 0)
						{
							dirCnt_++;
							if (nextDir_[dirCnt_] == DIR::MAX)
							{
								dirCnt_ = 0;
							}
							dir_ = nextDir_[dirCnt_];
							continue;
						}

						return true;
					}
				}

			}
		}
	}

	//TRACE("%d\n", static_cast<int>(dir_));
	return true;
}

bool Player::CheckWallInput(DIR dir)
{
	for (auto& layer : layerData_)
	{
		if (layer.name == "Obj")
		{
			Vector2 size{};

			switch (dir)
			{
			case DIR::LEFT:
				size = { pos_.x - vel_.x, pos_.y + 16 };
				break;
			case DIR::UP:
				size = { pos_.x + 16, pos_.y - vel_.y};
				break;
			case DIR::RIGHT:
				size = { pos_.x + 32,pos_.y + 16 };
				break;
			case DIR::DOWN:
				size = { pos_.x + 16,pos_.y + 32 };
				break;
			default:
				assert(!"エラー：PLAYERDIR");
				break;
			}
			size /= 32;

			// チップの番号に変換
			int num = size.x + size.y * layer.width;

			if (layer.chipData[num] != 0)
			{
				return false;
			}
		}
	}

	return true;
}

bool Player::UpdateDef()
{
	// 入力のアップデート
	(*input_)();

	auto input = input_->GetTrgData();
	bool animFlg = false;
	for (auto list = inputMoveList_.begin(); list != inputMoveList_.end(); list++)
	{
		if ((*list)(input, true))
		{
			inputMoveList_.splice(inputMoveList_.begin(), inputMoveList_, list);
			inputMoveList_.sort([&](MoveFunc& a, MoveFunc& b) {return a(input, false) < b(input, false); });
			state_ = STATE::Run;
			animFlg = true;
			break;
		}
	}

	// 何も入力されなかった
	if (!animFlg)
	{
		state_ = STATE::Non;
	}

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

bool Player::UpdateAuto()
{
	CheckWallAuto();
	pos_ += (dirMap_[dir_] * vel_);

	UnionData data[4]{};
	data[0].iData = id_;
	data[1].iData = pos_.x;
	data[2].iData = pos_.y;
	data[3].iData = static_cast<int>(dir_);

	lpNetWork.SendMes(MesType::POS, UnionVec{ data[0],data[1],data[2] ,data[3] });

	return true;
}

bool Player::UpdateRev()
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

void Player::InitFunc(void)
{
	// 補正
	auto correction = [&](int remainder, int& pos)
	{
		if (remainder >= 16)
		{
			pos += 32 - remainder;
		}
		else
		{
			pos -= remainder;
		}
	};

	// 左方向処理
	auto left= [&](TrgData trgData, bool flg)
	{
		if (trgData[INPUT_ID::BUTTON_LEFT][static_cast<int>(Trg::Now)])
		{
			dir_ = DIR::LEFT;
			if (flg)
			{
				if (CheckWallInput(dir_))
				{
					correction((pos_.y) % 32, pos_.y);
					pos_.x -= vel_.x;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	};

	// 右方向処理
	auto right= [&](TrgData trgData, bool flg)
	{
		if (trgData[INPUT_ID::BUTTON_RIGHT][static_cast<int>(Trg::Now)])
		{
			dir_ = DIR::RIGHT;
			if (flg)
			{
				if (CheckWallInput(dir_))
				{
					correction((pos_.y) % 32, pos_.y);
					pos_.x += vel_.x;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;	
	};

	// 上方向処理
	auto up= [&](TrgData trgData, bool flg)
	{
		if (trgData[INPUT_ID::BUTTON_UP][static_cast<int>(Trg::Now)])
		{
			dir_ = DIR::UP;
			if (flg)
			{
				if (CheckWallInput(dir_))
				{
					correction((pos_.x) % 32, pos_.x);
					pos_.y -= vel_.y;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	};

	// 下方向処理
	auto down= [&](TrgData trgData, bool flg)
	{
		if (trgData[INPUT_ID::BUTTON_DOWN][static_cast<int>(Trg::Now)])
		{
			dir_ = DIR::DOWN;
			if (flg)
			{
				if (CheckWallInput(dir_))
				{
					correction((pos_.x) % 32, pos_.x);
					pos_.y += vel_.y;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	};

	inputMoveList_.emplace_back(left);
	inputMoveList_.emplace_back(right);
	inputMoveList_.emplace_back(up);	
	inputMoveList_.emplace_back(down);
}