#include <algorithm>
#include <random>
#include <DxLib.h>
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "Stage.h"
#include "Scene/SceneMng.h"
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
#include "ModePuyo/Victory/Lose.h"
#include "ModePuyo/Victory/Win.h"
#include "ModePuyo/Victory/Draw.h"
#include "NextPuyo.h"
#include "common/EffectMng.h"

int Stage::playCnt_ = 0;
std::map<int, int> Stage::stgBG_;

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

const bool Stage::alive(void) const
{
	return alive_;
}

const bool Stage::nextScene(void) const
{
	return nextScene_;
}

const Victory& Stage::victory(void) const
{
	return victory_;
}

const bool Stage::CheckMenuScene(void) const
{
	return false;
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
				lpEffectMng.PlayEffect("baku2", offSet_ + puyo->pos() - Vector2{ 0,puyo->size() });
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

void Stage::gameOverCnt(int cnt)
{
	gameOverCnt_ = cnt;
}

void Stage::victory(Victory vic)
{
	victory_ = vic;
}

void Stage::DrawUpdate(void)
{
	// 背景描画
	//SetDrawScreen(stageID_);
	//ClsDrawScreen();
	//DrawBox(0, 0, size_.x + 1, size_.y, 0x778899, true);

	// プヨ描画
	SetDrawScreen(puyoID_);
	ClsDrawScreen();
	for (auto&& puyo : puyoVec_)
	{
		puyo->PuyonUpdate();
		puyo->Draw();
	}

	// プヨ描画
	SetDrawScreen(ojamaID_);
	ClsDrawScreen();
	// お邪魔リストの描画
	for (auto&& ojama : ojamaList_)
	{
		ojama->PuyonUpdate();
		ojama->Draw();
	}

	// ぷよの周りを光らせるガイドの描画
	SetDrawScreen(guideID_);
	ClsDrawScreen();

	if (puyoVec_.size())
	{
		// 下に行けるときだけ操作ぷよに枠を付ける
		if (puyoVec_[playUnit_->target()]->id() != PuyoID::Ojama)
		{
			if (!puyoVec_[playUnit_->target()]->GetDirFlg().bit.down)
			{
				DrawOval(puyoVec_[playUnit_->target()]->drawPos().first.x, puyoVec_[playUnit_->target()]->drawPos().first.y,
					puyoVec_[playUnit_->target()]->drawPos().second.x - 20, puyoVec_[playUnit_->target()]->drawPos().second.y - 20, 0x797979, true);
			}
		}
	}

	lpEffectMng.Draw();
}

void Stage::DrawStage(void)
{
	Vector2 tmpPos{ size().x / 4 + 4, -blockSize_ / 2 };
	// 358 417
	Vector2 drawPos{ offSet() + size() / 2 + gameOverPos_ };
	lpSceneMng.AddDrawQue({ puyoID_,drawPos.x + tmpPos.x, drawPos.y + tmpPos.y,1.0, (float)angle_, 2 });
	lpSceneMng.AddDrawQue({ guideID_,drawPos.x + tmpPos.x, drawPos.y + tmpPos.y,1.0, (float)angle_, 1});

	tmpPos = { 0, blockSize_/2};
	lpSceneMng.AddDrawQue({ stgBG_[id_],drawPos.x + tmpPos.x, drawPos.y + tmpPos.y,1.0,(float)angle_,  5});

	tmpPos = { size().x / 4 + 4, blockSize_ / 2 - 1 };
	lpSceneMng.AddDrawQue({ ojamaID_ ,drawPos.x + tmpPos.x, drawPos.y + tmpPos.y,1.0 ,0.0f,-10 });

	lpSceneMng.AddDrawQue({ hideID_ ,offSet().x - 16,offSet().y - 16,1.0 ,0.0f,0 });
	nextPuyo_->Draw();
}

int Stage::Update(int ojamaCnt)
{
	int count = 0;

	while (count < ojamaCnt)
	{
		InstanceOjama(count);
		count++;
	}

	if (count)
	{
		// 新しくインスタンスされてたら全体をリセットする
		int reset = 0;
		for (auto ojama : ojamaList_)
		{
			ojama->Init(reset);
			reset++;
		}
	}

	ojamaCnt_ = 0;

	// 操作対象の切り替え(PGUP : 1P, PGDN : 2P)
	int pgDn = CheckHitKey(KEY_INPUT_PGDN);
	if (CheckHitKey(KEY_INPUT_PGUP) || pgDn)
	{
		int no = 0;
		if (pgDn)
		{
			no = 1;
		}
		TRACE("操作対象の入れ替え\n");
		for (auto key : input_)
		{
			key.second->SetUp(id_ ^ no);
		}
	}

	// 操作処理
	for (auto key : input_)
	{
		(*key.second)();
	}

	// 勝ち負けが決まったらモードを移動
	if (victory_ != Victory::Non)
	{
		stgMode_ = StgMode::GameOver;
	}

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

	victoryMap_.try_emplace(Victory::Draw,Draw());
	victoryMap_.try_emplace(Victory::Lose,Lose());
	victoryMap_.try_emplace(Victory::Win,Win());

	victory_ = Victory::Non;
	blockSize_ = 64;
	angle_ = 0;
	rensa_ = 0;
	gameOverCnt_ = 300;
	gameOverPos_ = { 0,0 };
	alive_ = true;

	stageID_ = MakeScreen(size_.x + 200, size_.y,true);
	ojamaPuyoID_ = MakeScreen(size_.x + 200, size_.y, true);
	puyoID_ = MakeScreen(size_.x + 200, size_.y + blockSize_, true);
	guideID_ = MakeScreen(size_.x + 200, size_.y + blockSize_, true);
	hideID_ = MakeScreen(lpSceneMng.screenSize_.x, 64, true);
	ojamaID_ = MakeScreen(size_.x + 200, size_.y + blockSize_, true);

	dataBase_.resize(STAGE_Y * STAGE_X );										// 全体のサイズを作る
	eraseDataBase_.resize(STAGE_Y * STAGE_X);									// 全体のサイズを作る

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

	// 操作系
	for (auto id : INPUT_ID())
	{
		moveFlg_.try_emplace(id, false);
	}

	input_ =
	{ 
		{CntType::Pad, std::make_shared<PadState>()},
		//{CntType::Mouse, std::make_shared<MouseState>()},
		{CntType::Key, std::make_shared<KeyState>()}
	};

	for (auto key : input_)
	{
		key.second->SetUp(id_);
	}	

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

	stgBG_.try_emplace(0, lpImageMng.GetHandle("Stage03")[0]);
	stgBG_.try_emplace(1, lpImageMng.GetHandle("Stage03")[0]);

	nextPuyo_ = std::make_unique<NextPuyo>(pos, 2);
	nextScene_ = false;
	InstancePuyo();

	// プヨ隠す用のスクリーンをあらかじめ作成しておく
	SetDrawScreen(hideID_);
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetHandle("BG")[0], true);


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

void Stage::InstanceOjama(int no)
{
	ojamaList_.emplace_front(std::make_shared<OjamaPuyo>(Vector2(blockSize_ / 2 + blockSize_ * 3, blockSize_ / 2), no));
}