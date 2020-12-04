#include "Player.h"
#include <DxLib.h>
#include <chrono>
#include <cassert>
#include "../common/ImageMng.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../NetWork/NetWork.h"
#include "../Scene/GameScene.h"
#include "../Input/PadState.h"
#include "../Input/KeyState.h"

int Player::lostCnt_ = 0;

Player::Player(Vector2& pos, BaseScene& scene,LayerVec& layer, int id):scene_(scene),layerData_(layer)
{
	lpNetWork.AddRevList(revMutex_, revList_);
	pos_ = pos;
	vel_ = { 4,4 };
	rad_ = 0;
	dir_ = DIR::DOWN;
	animCnt_ = 0;
	dirCnt_ = 0;
	state_ = STATE::Non;
	length_ = 3;
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

	int modeID = lpNetWork.GetNetWorkMode() != NetWorkMode::HOST ? 1 : 0;

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
		if (id_ == lpNetWork.GetPlayerID())
		{
			netFunc_ = std::bind(&Player::UpdateDef,this);
			InitFunc();
		}
		else
		{
			netFunc_ = std::bind(&Player::UpdateRev, this);
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
	auto bombMap = dynamic_cast<GameScene&>(scene_).GetBombMap();
	if (pos_.x % 32 == 0)
	{
		if (pos_.y % 32 == 0)
		{
			for (auto& layer : layerData_)
			{
				if (layer.name == "Obj")
				{
					int count = 0;
					while (count < 4)
					{
						Vector2 size{ (pos_.x / 32),(pos_.y / 32) };
						size += dirMap_[dir_];
						int num = ((size.x) + (size.y) * layer.width);

						if (layer.chipData[num] != 0 || bombMap[num])
						{
							dirCnt_++;
							if (nextDir_[dirCnt_] == DIR::MAX)
							{
								dirCnt_ = 0;
							}
							dir_ = nextDir_[dirCnt_];
							count++;
							continue;
						}
						return true;
					}
					return false;
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

bool Player::CheckMoveBombAuto(void)
{
	auto bombMap = dynamic_cast<GameScene&>(scene_).GetBombMap();

	return false;
}

bool Player::CheckMoveBomb(DIR dir)
{
	auto bombMap = dynamic_cast<GameScene&>(scene_).GetBombMap();

	Vector2 size{};
	switch (dir)
	{
	case DIR::LEFT:
		size = { pos_.x - vel_.x, pos_.y + 16 };
		break;
	case DIR::UP:
		size = { pos_.x + 16, pos_.y - vel_.y };
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
	int num = size.x + size.y * 21;

	if (bombMap[num])
	{
		return false;
	}
	return true;
}

bool Player::UpdateDef()
{
	if (!alive_)
	{
		return false;
	}

	// 入力のアップデート
	(*input_)();

	auto input = input_->GetTrgData();
	bool animFlg = false;
	for (auto list = inputMoveList_.begin(); list != inputMoveList_.end(); list++)
	{
		if ((*list)(input, true))
		{
			inputMoveList_.splice(inputMoveList_.begin(), inputMoveList_, list);
			inputMoveList_.sort([&](MoveFuncInput& a, MoveFuncInput& b) {return a(input, false) < b(input, false); });
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

	// 爆弾設置
	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_ATTACK))
	{
		auto no = CheckBomb();
		if (no >= 0)
		{
			try
			{
				chronoTime time = std::chrono::system_clock::now();
				Vector2 pos{ (pos_.x + 16) / 32 ,(pos_.y + 16) / 32 };
				pos = { pos.x * 32,pos.y * 32 };
				dynamic_cast<GameScene&>(scene_).SetBomb(id_, no,pos ,time, length_,true);
			}
			catch (...)
			{
				assert(!"シーンのキャスト失敗");
			}
		}
	}

	int chipPos = (pos_.y / 32) * 21 + (pos_.x / 32);

	if (dynamic_cast<GameScene&>(scene_).CheckHitFlame(chipPos))
	{

		UnionData data[1]{};
		data[0].iData = id_;

		lpNetWork.SendMesAll(MesType::DETH, UnionVec{ data[0]}, 0);
		alive_ = false;
	}
	else
	{

		UnionData data[4]{};
		data[0].iData = id_;
		data[1].iData = pos_.x;
		data[2].iData = pos_.y;
		data[3].iData = static_cast<int>(dir_);

		lpNetWork.SendMesAll(MesType::POS, UnionVec{ data[0],data[1],data[2] ,data[3] }, 0);
	}

	return true;
}

bool Player::UpdateAuto()
{
	//CheckMoveBombAuto();
	if (CheckWallAuto())
	{
		state_ = STATE::Run;
		pos_ += (dirMap_[dir_] * vel_);
	}
	else
	{
		state_ = STATE::Non;
	}

	int chipPos = (pos_.y / 32) * 21 + (pos_.x / 32);
	if (dynamic_cast<GameScene&>(scene_).CheckHitFlame(chipPos))
	{

		UnionData data[1]{};
		data[0].iData = id_;

		lpNetWork.SendMes(MesType::DETH, UnionVec{ data[0] });
		alive_ = false;
	}
	else
	{
		UnionData data[4]{};
		data[0].iData = id_;
		data[1].iData = pos_.x;
		data[2].iData = pos_.y;
		data[3].iData = static_cast<int>(dir_);

		lpNetWork.SendMes(MesType::POS, UnionVec{ data[0],data[1],data[2] ,data[3] });
	}

	return true;
}

bool Player::UpdateRev()
{
	bool lost = false;
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
		lost = true;     
	}
	
	while (CheckData(MesType::SET_BOMB))
	{
		UnionVec data{};
		PickData(MesType::SET_BOMB, data);

		if (data.size())
		{
			Vector2 pos{ data[2].iData,data[3].iData };
			Vector2 sendPos{ (pos.x) / 32 ,(pos.y) / 32 };
			sendPos = { sendPos.x * 32,sendPos.y * 32 };
			try
			{
				TimeData timeData{};
				timeData.iData[0] = data[5].iData;
				timeData.iData[1] = data[6].iData;

				if (data[0].iData / 5 > lpNetWork.GetPlayerMax())
				{
					TRACE("エラー:ownerID : %d\n", data[0].iData / 5);
					break;
				}

				if (data[1].iData / 5 > lpNetWork.GetPlayerMax())
				{
					TRACE("エラー:myID : %d\n", data[1].iData / 5);
					break;
				}

				if (data[4].iData > MAX_FIRE_LENGTH)
				{
					TRACE("エラー:length : %d\n", data[1].iData / 5);
					break;
				}

				dynamic_cast<GameScene&>(scene_).SetBomb(data[0].iData, data[1].iData, sendPos,timeData.time, data[4].iData,false);
			}
			catch (...)
			{
				int a = 0;
			}
		}
		lost = true;
	}

	if (CheckData(MesType::DETH) || CheckData(MesType::LOST))
	{
		UnionVec data{};
		PickData(MesType::DETH, data);

		alive_ = false;
		lost = true;
	}

	if (!lost)
	{
		lostCnt_++;
		// TRACE("取りこぼしID:%d\n", id_);
	}

	return true;
}

void Player::AddBombList(int no)
{
	if (alive_)
	{
		bombList_.emplace_back(no);
	}
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

	// ------------ Input ----------------------------------------------

	// 左方向処理
	auto inputLeft= [&](TrgData trgData, bool flg)
	{
		if (trgData[INPUT_ID::BUTTON_LEFT][static_cast<int>(Trg::Now)])
		{
			dir_ = DIR::LEFT;
			if (flg)
			{
				if (CheckWallInput(dir_) && CheckMoveBomb(dir_))
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
	auto inputRight= [&](TrgData trgData, bool flg)
	{
		if (trgData[INPUT_ID::BUTTON_RIGHT][static_cast<int>(Trg::Now)])
		{
			dir_ = DIR::RIGHT;
			if (flg)
			{
				if (CheckWallInput(dir_) && CheckMoveBomb(dir_))
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
	auto inputUp= [&](TrgData trgData, bool flg)
	{
		if (trgData[INPUT_ID::BUTTON_UP][static_cast<int>(Trg::Now)])
		{
			dir_ = DIR::UP;
			if (flg)
			{
				if (CheckWallInput(dir_) && CheckMoveBomb(dir_))
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
	auto inputDown= [&](TrgData trgData, bool flg)
	{
		if (trgData[INPUT_ID::BUTTON_DOWN][static_cast<int>(Trg::Now)])
		{
			dir_ = DIR::DOWN;
			if (flg)
			{
				if (CheckWallInput(dir_) && CheckMoveBomb(dir_))
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

	inputMoveList_.emplace_back(inputLeft);
	inputMoveList_.emplace_back(inputRight);
	inputMoveList_.emplace_back(inputUp);	
	inputMoveList_.emplace_back(inputDown);


	// ------------ Auto ----------------------------------------------

}