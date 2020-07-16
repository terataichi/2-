#include <algorithm>
#include <DxLib.h>
#include "_debug/_DebugConOut.h"
#include "Stage.h"
#include "SceneMng.h"
#include "Input/KeyState.h"
#include "Input/PadState.h"
#include "Input/MouseState.h"

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

void Stage::Draw(void)
{
	SetDrawScreen(stageID_);
	ClsDrawScreen();
	DrawBox(0,0,lpSceneMng.gameSize_.x,lpSceneMng.gameSize_.y, 0xfff, true);
	for (auto puyo : puyo_)
	{
		puyo->Draw();
	}
}

void Stage::UpDate(void)
{
	(*input_)();


	DirUnion _dirFlg;												// 移動していいかどうかの情報を作る
	Vector2 grid = puyo_[0]->GetGrid(blockSize_) + 1;
	_dirFlg.bit = { data_[grid.y][grid.x - 1] != PuyoID::Non,
					data_[grid.y - 1][grid.x] != PuyoID::Non,
					data_[grid.y][grid.x + 1] != PuyoID::Non,
					data_[grid.y + 1][grid.x] != PuyoID::Non };
	if (_dirFlg.bit.down)
	{
		puyo_.emplace(puyo_.begin(), std::make_shared<puyo>());
		data_[grid.y][grid.x] = puyo_[0]->id();
	}
	puyo_[0]->SetDirFlg(_dirFlg);
	for (auto data : input_->GetTrgData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			puyo_[0]->Move(data.first);
		}
	}


	puyo_[0]->UpDate();
	Draw();
}

void Stage::Init()
{
	stageID_ = MakeScreen(size_.x, size_.y);

	dataBase_.resize(STAGE_Y * STAGE_X );							// 全体のサイズを作る

	//_data.resize(STAGE_Y);										// Yのサイズを確保してそこにXを格納していく
	//for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	//{
	//	_data[j] = &_dataBase[j * STAGE_X];
	//}

	//_dataBase[0] = 500;
	for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	{
		data_.emplace_back(&dataBase_[j * STAGE_X]);
		for (int i = 0; i < static_cast<int>(STAGE_X); i++)
		{
			if (i == 0 || i == static_cast<int>(STAGE_X - 1)||
				j == 0 || j == static_cast<int>(STAGE_Y - 1))
			{
				data_[j][i] = PuyoID::Wall;
			}
		}
	}
	TRACE("%d\n", data_[0][0]);

	for (auto id : INPUT_ID())
	{
		moveFlg_.try_emplace(id, false);
	}

	blockSize_ = 64;
	input_ = std::make_shared<KeyState>();
	input_->SetUp(id_);
	puyo_.emplace(puyo_.begin(), std::make_shared<puyo>());
}
