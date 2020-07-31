#include <algorithm>
#include <functional>
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
#include "ModePuyo/DropMode.h"
#include "ModePuyo/FallMode.h"
#include "ModePuyo/PuyonMode.h"
#include "ModePuyo/RensaMode.h"
#include "ModePuyo/MunyonMode.h"
#include "ModePuyo/GameOver.h"


int Stage::playCnt_ = 0;

Stage::Stage(Vector2&& offSet,Vector2&& size) :offSet_(std::move(offSet)), size_(std::move(size))
{
	id_ = playCnt_;
	Init();
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

const Vector2 Stage::GetGrid(Vector2 pos)
{
	return Vector2((pos.x + blockSize_ / 2) / blockSize_, (pos.y + blockSize_ / 2) / blockSize_);
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
				if (data_[vec.y][vec.x]->id() == id && id != PuyoID::Wall)
				{
					erasedata_[vec.y][vec.x] = data_[vec.y][vec.x];
					count++;
					erasePuyo(id, Vector2(vec.x - 1, vec.y));
					erasePuyo(id, Vector2(vec.x, vec.y - 1));
					erasePuyo(id, Vector2(vec.x + 1, vec.y));
					erasePuyo(id, Vector2(vec.x, vec.y + 1));
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
				puyo->alive(false);
				data_[gri.y][gri.x].reset();
			}
		}
	}
	return true;
}

void Stage::Draw(void)
{
	SetDrawScreen(stageID_);
	ClsDrawScreen();
	DrawBox(0, 0, lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y, 0x778899, true);
	for (auto&& puyo : puyoVec_)
	{
		puyo->Draw();
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

void Stage::UpDate(void)
{
	(*input_)();
	modeMap_[stgMode_](*this);
	Draw();
}

void Stage::Init()
{

	modeMap_.try_emplace(StgMode::Drop, DropMode());
	modeMap_.try_emplace(StgMode::FALL, FallMode());
	modeMap_.try_emplace(StgMode::Puyon, PuyonMode());
	modeMap_.try_emplace(StgMode::Rensa, RensaMode());
	modeMap_.try_emplace(StgMode::Munyon, MunyonMode());
	modeMap_.try_emplace(StgMode::GameOver, GameOver());

	stageID_ = MakeScreen(size_.x, size_.y);

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

	blockSize_ = 64;
	input_ = std::make_shared<KeyState>();
	input_->SetUp(id_);
	playUnit_ = std::make_unique<PlayUnit>(*this);
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
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> puyoRand(static_cast<int>(PuyoID::Red), static_cast<int>(PuyoID::Purple));

	puyoVec_.emplace(puyoVec_.begin(), std::make_shared<puyo>(Vector2(blockSize_ / 2 + blockSize_ * 3, blockSize_ + blockSize_ / 2), static_cast<PuyoID>(puyoRand(mt))));
	puyoVec_.emplace(puyoVec_.begin(), std::make_shared<puyo>(Vector2(blockSize_ / 2 + blockSize_ * 3, blockSize_ / 2), static_cast<PuyoID>(puyoRand(mt))));
	playUnit_->SetTarget();
}
