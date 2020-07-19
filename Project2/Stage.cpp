#include <algorithm>
#include <functional>
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

bool Stage::SetErase(void)
{
	memset(eraseDataBase_.data(), 0, eraseDataBase_.size() * sizeof(PuyoID));		// サイズを作って中に0(PuyoID::Non)をいれる

	Vector2 grid = puyoVec_[0]->GetGrid(blockSize_);								// 起点のマスを設定
	int count = 0;																	// 隣に同じIDが見つかればプラス

	// 再起関数
	std::function<void(PuyoID, Vector2)> erasePuyo = [&](PuyoID id, Vector2 vec)
	{
		if (erasedata_[vec.y][vec.x] == PuyoID::Non)
		{
			if (data_[vec.y][vec.x] == id)
			{
				erasedata_[vec.y][vec.x] = data_[vec.y][vec.x];
				count++;
				erasePuyo(id, Vector2(vec.x - 1, vec.y));
				erasePuyo(id, Vector2(vec.x, vec.y - 1));
				erasePuyo(id, Vector2(vec.x + 1, vec.y));
				erasePuyo(id, Vector2(vec.x, vec.y + 1));
			}
		}
	};
	erasePuyo(puyoVec_[0]->id(), grid);

	if (count < 4)
	{
		for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
		{
			for (int i = 0; i < static_cast<int>(STAGE_X); i++)
			{
				erasedata_[j][i] = PuyoID::Non;
			}
		}
		return false;
	}

	for (auto&& puyo : puyoVec_)
	{
		Vector2 gri = puyo->GetGrid(blockSize_);
		if (erasedata_[gri.y][gri.x] == puyo->id())
		{
			puyo->alive(false);
			data_[gri.y][gri.x] = PuyoID::Non;
		}
	}
	return true;
}

void Stage::Draw(void)
{
	SetDrawScreen(stageID_);
	ClsDrawScreen();
	DrawBox(0,0,lpSceneMng.gameSize_.x,lpSceneMng.gameSize_.y, 0xffffff, true);
	for (auto&& puyo : puyoVec_)
	{
		puyo->Draw();
	}
}

void Stage::UpDate(void)
{
	(*input_)();
	
	DirUnion _dirFlg;												// 移動していいかどうかの情報を作る
	Vector2 grid = puyoVec_[0]->GetGrid(blockSize_);
	_dirFlg.bit = { data_[grid.y][grid.x - 1] != PuyoID::Non,
					data_[grid.y - 1][grid.x] != PuyoID::Non,
					data_[grid.y][grid.x + 1] != PuyoID::Non,
					data_[grid.y + 1][grid.x] != PuyoID::Non };
	puyoVec_[0]->SetDirFlg(_dirFlg);

	if (puyoVec_[0]->UpDate())
	{
		data_[grid.y][grid.x] = puyoVec_[0]->id();
		if (SetErase())
		{
			auto itr = std::remove_if(puyoVec_.begin(), puyoVec_.end(), [](std::unique_ptr<puyo>& puyo) {return !(puyo->alive()); });
			puyoVec_.erase(itr, puyoVec_.end());
		}
		puyoVec_.emplace(puyoVec_.begin(), std::make_unique<puyo>());
	}

	if (input_->GetTrgData().at(INPUT_ID::BUTTON_DOWN)[static_cast<int>(Trg::Now)])
	{
		puyoVec_[0]->SoftDrop();
	}
	for (auto data : input_->GetTrgData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			puyoVec_[0]->Move(data.first);
		}
	}

	Draw();
}

void Stage::Init()
{
	stageID_ = MakeScreen(size_.x, size_.y);

	dataBase_.resize(STAGE_Y * STAGE_X );							// 全体のサイズを作る
	eraseDataBase_.resize(STAGE_Y * STAGE_X);							// 全体のサイズを作る

	//_data.resize(STAGE_Y);										// Yのサイズを確保してそこにXを格納していく
	//for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	//{
	//	_data[j] = &_dataBase[j * STAGE_X];
	//}

	//_dataBase[0] = 500;
	for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	{
		data_.emplace_back(&dataBase_[j * STAGE_X]);
		erasedata_.emplace_back(&eraseDataBase_[j * STAGE_X]);
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
	puyoVec_.emplace(puyoVec_.begin(), std::make_unique<puyo>());
}
