#include <algorithm>
#include <random>
#include <DxLib.h>
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "Stage.h"
#include "SceneMng.h"
#include "Input/KeyState.h"
#include "Input/PadState.h"
#include "Input/MouseState.h"
#include "PlayUnit.h"
#include "OjamaPuyo.h"
#include "ModePuyo/DropMode.h"
#include "ModePuyo/FallMode.h"
#include "ModePuyo/PuyonMode.h"
#include "ModePuyo/RensaMode.h"
#include "ModePuyo/MunyonMode.h"
#include "ModePuyo/GameOver.h"
#include "ModePuyo/DeleteMode.h"
#include "NextPuyo.h"
#include "common/EffectMng.h"

int Stage::playCnt_ = 0;

Stage::Stage(Vector2&& offSet,Vector2&& size) :offSet_(std::move(offSet)), size_(std::move(size))
{
	id_ = playCnt_;
	Init();
	maxRensa_ = 0;
	eraseCnt_ = 0;
	playCnt_++;
}

Stage::~Stage()
{
	playCnt_--;
}

const int Stage::GetStageID(void) const
{
	return Stage::stageID_;
}

const Vector2 Stage::offSet(void) const
{
	return Stage::offSet_;
}

const Vector2 Stage::size(void) const
{
	return Stage::size_;
}

const int Stage::rensa(void) const
{
	return rensa_;
}

const Vector2 Stage::GetGrid(Vector2 pos)
{
	return Vector2((pos.x + blockSize_ / 2) / blockSize_, (pos.y + blockSize_ / 2) / blockSize_);
}

const int Stage::ojamaCnt(void) const
{
	return ojamaCnt_;
}

const int Stage::id(void) const
{
	return id_ + 1;
}

bool Stage::SetErase(SharePuyo& puyo, Vector2 vec)
{
	//memset(eraseDataBase_.data(), 0, eraseDataBase_.size() * sizeof(PuyoID));		// サイズを作って中に0(PuyoID::Non)をいれる

	for (auto& puyo : eraseDataBase_)
	{
		puyo.reset();
	}

	Vector2 grid = puyo->GetGrid(blockSize_);										// 起点のマスを設定
	int count = 0;																	// 隣に同じIDが見つかればプラス

	// 再起関数
	std::function<void(PuyoID, Vector2)> erasePuyo = [&](PuyoID id, Vector2 vec)
	{
		if (!erasedata_[vec.y][vec.x])
		{
			if (data_[vec.y][vec.x])
			{
				if (data_[vec.y][vec.x]->id() == id && id != PuyoID::Wall && id != PuyoID::Ojama)
				{
					erasedata_[vec.y][vec.x] = data_[vec.y][vec.x];
					count++;
					erasePuyo(id, Vector2(vec.x - 1, vec.y));
					erasePuyo(id, Vector2(vec.x, vec.y - 1));
					erasePuyo(id, Vector2(vec.x + 1, vec.y));
					erasePuyo(id, Vector2(vec.x, vec.y + 1));
				}
				if (data_[vec.y][vec.x]->id() == PuyoID::Ojama)
				{
					erasedata_[vec.y][vec.x] = data_[vec.y][vec.x];
				}
			}
		}
	};
	erasePuyo(puyo->id(), grid);

	if (count < 4)
	{
		for (auto& puyo : eraseDataBase_)
		{
			puyo.reset();
		}
		return false;
	}

	for (auto&& puyo : puyoVec_)
	{
		Vector2 gri = puyo->GetGrid(blockSize_);

		if (erasedata_[gri.y][gri.x])
		{
			if (erasedata_[gri.y][gri.x]->id() == puyo->id())
			{
				lpEffectMng.PlayEffect("blast", offSet_ + puyo->pos() + puyo->rad());
				puyo->alive(false);
				data_[gri.y][gri.x].reset();
			}
		}
	}

	eraseCnt_ += count;

	return true;
}

void Stage::ResetRensa(void)
{
	rensa_ = 0;
}

void Stage::AddRensa(void)
{
	rensa_++;
}

void Stage::ojamaCnt(int cnt)
{
	ojamaCnt_ = cnt;
}

void Stage::DrawUpdate(void)
{
	SetDrawScreen(stageID_);
	ClsDrawScreen();
	DrawBox(0, 0, lpSceneMng.gameSize_.x + 1, lpSceneMng.gameSize_.y, 0x778899, true);
	for (auto&& puyo : puyoVec_)
	{
		puyo->Draw();
	}
	for (auto&& ojama : ojamaList_)
	{
		ojama->Draw();
	}

	//for (auto data : data_)
	//{
	//	if ((*data))
	//	{
	//		if ((*data)->id() == PuyoID::Wall)
	//		{
	//			DrawBox((*data)->pos().x, (*data)->pos().x, (*data)->pos().x + blockSize_, (*data)->pos().y + blockSize_, 0xf, false);
	//		}
	//	}
	//}
}

void Stage::DrawStage(void)
{
	DrawGraph(offSet().x, offSet().y, GetStageID(), true);
	nextPuyo_->Draw();
	lpEffectMng.Draw();
}

int Stage::UpDate(int ojamaCnt)
{
	int count = 0;
	while (count < ojamaCnt)
	{
		InstanceOjama();
		count++;
	}
	ojamaCnt_ = 0;
	(*input_)();
	modeMap_[stgMode_](*this);
	DrawUpdate();

	return ojamaCnt_;
}

void Stage::Init()
{

	modeMap_.try_emplace(StgMode::Drop, DropMode());
	modeMap_.try_emplace(StgMode::FALL, FallMode());
	modeMap_.try_emplace(StgMode::Puyon, PuyonMode());
	modeMap_.try_emplace(StgMode::Rensa, RensaMode());
	modeMap_.try_emplace(StgMode::Munyon, MunyonMode());
	modeMap_.try_emplace(StgMode::GameOver, GameOver());
	modeMap_.try_emplace(StgMode::Delete, DeleteMode());
	//modeMap_.try_emplace(StgMode::Ojama, OjamaMode());

	stageID_ = MakeScreen(size_.x + 200, size_.y);

	dataBase_.resize(STAGE_Y * STAGE_X );							// 全体のサイズを作る
	eraseDataBase_.resize(STAGE_Y * STAGE_X);						// 全体のサイズを作る

	for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	{
		data_.emplace_back(&dataBase_[j * STAGE_X]);
		erasedata_.emplace_back(&eraseDataBase_[j * STAGE_X]);
		for (int i = 0; i < static_cast<int>(STAGE_X); i++)
		{
			if (i == 0 || i == static_cast<int>(STAGE_X - 1)||
				j == 0 || j == static_cast<int>(STAGE_Y - 1))
			{
				data_[j][i] = std::make_shared<puyo>(Vector2(blockSize_ * i, blockSize_ * j), PuyoID::Wall);
			}
		}
	}

	for (auto id : INPUT_ID())
	{
		moveFlg_.try_emplace(id, false);
	}

	rensa_ = 0;
	blockSize_ = 64;
	input_ = std::make_shared<KeyState>();
	input_->SetUp(id_);
	playUnit_ = std::make_unique<PlayUnit>(*this);
	ojamaList_.clear();
	Vector2 pos;
	if (id_ == 0)
	{
		pos = offSet_ + Vector2{ (size_.x + 1) + blockSize_ / 2,blockSize_ };
	}
	else
	{
		pos = offSet_ + Vector2{ -blockSize_ * 2,blockSize_ };
	}
	nextPuyo_ = std::make_unique<NextPuyo>(pos, 2);
	InstancePuyo();

	stgMode_ = StgMode::Drop;
}

bool Stage::CheckMove(SharePuyo& vec)
{
	bool next = true;
	DirUnion dirFlg;												// 移動していいかどうかの情報を作る
	Vector2 grid = vec->GetGrid(blockSize_);
	dirFlg.bit = {1,1,1,1};

	// 中途半端な時に移動するとめり込むので余りが出たらもう一個下を見る
	int set = (vec->pos().y + vec->rad().y) % blockSize_ != 0;

	if (!data_[grid.y + set][grid.x - 1])
	{
		dirFlg.bit.left = 0;
	}
	if (!data_[grid.y - 1][grid.x])
	{
		dirFlg.bit.up = 0;
	}
	if (!data_[grid.y + set][grid.x + 1])
	{
		dirFlg.bit.right = 0;
	}
	if (!data_[grid.y + 1][grid.x])
	{
		dirFlg.bit.down = 0;
	}

	if (dirFlg.bit.down)
	{
		data_[grid.y][grid.x] = vec;
		next = false;
		vec->SetDirFlg(dirFlg);
		return next;
	}
	vec->SetOldDirFlg();
	vec->SetDirFlg(dirFlg);
	return next;
}

void Stage::InstancePuyo()
{
	auto nextPuyo = nextPuyo_->PickUp();

	auto pos2 = Vector2{ blockSize_ / 2 + blockSize_ * 3, blockSize_ + blockSize_ / 2 };
	auto pos1 = Vector2{blockSize_ / 2 + blockSize_ * 3, blockSize_ / 2};
	nextPuyo.first->pos(pos1);
	nextPuyo.second->pos(pos2);

	puyoVec_.emplace(puyoVec_.begin(), nextPuyo.first);
	puyoVec_.emplace(puyoVec_.begin(), nextPuyo.second);

	playUnit_->SetTarget();
}

void Stage::InstanceOjama()
{
	ojamaList_.emplace_front(std::make_shared<OjamaPuyo>(Vector2(blockSize_ / 2 + blockSize_ * 3, blockSize_ / 2), PuyoID::Ojama));
}