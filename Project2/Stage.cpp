#include <algorithm>
#include <DxLib.h>
#include "_debug/_DebugConOut.h"
#include "Stage.h"
#include "SceneMng.h"
#include "Input/KeyState.h"
#include "Input/PadState.h"
#include "Input/MouseState.h"

int Stage::playCnt = 0;

Stage::Stage(Vector2&& offSet,Vector2&& size) :_offSet(std::move(offSet)), _size(std::move(size))
{
	_id = playCnt;
	Init();
	playCnt++;
}

Stage::~Stage()
{
	playCnt--;
}

const int Stage::GetStageID(void) const
{
	return Stage::_stageID;
}

const Vector2 Stage::offSet(void) const
{
	return Stage::_offSet;
}

const Vector2 Stage::size(void) const
{
	return Stage::_size;
}

void Stage::Draw(void)
{
	SetDrawScreen(_stageID);
	ClsDrawScreen();
	DrawBox(0,0,lpSceneMng._gameSize.x,lpSceneMng._gameSize.y, 0xfff, true);
	_puyo->Draw();
}

void Stage::UpDate(void)
{

	(*_input)();
	_puyo->UpDate();

	Vector2 tmpPos = (_puyo->pos() / _puyo->size());
	_moveFlg[INPUT_ID::BUTTON_LEFT] = tmpPos.x - 1 < 0;
	_moveFlg[INPUT_ID::BUTTON_RIGHT] = tmpPos.x + 1 > static_cast<int>(STAGE_X - 1);
	_moveFlg[INPUT_ID::BUTTON_UP] = tmpPos.y - 1 < 0;
	_moveFlg[INPUT_ID::BUTTON_DOWN] = tmpPos.y + 1 > static_cast<int>(STAGE_Y - 1);


	for (auto data : _input->GetTrgData())
	{
		if (!_moveFlg[data.first])
		{
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				_puyo->Move(data.first);
			}
		}
	}

	Draw();
}

void Stage::Init()
{
	_stageID = MakeScreen(_size.x, _size.y);

	_dataBase.resize(STAGE_Y * STAGE_X);							// 全体のサイズを作る

	//_data.resize(STAGE_Y);										// Yのサイズを確保してそこにXを格納していく
	//for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	//{
	//	_data[j] = &_dataBase[j * STAGE_X];
	//}

	//_dataBase[0] = 500;
	for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	{
		_data.emplace_back(&_dataBase[j * STAGE_X]);
	}
	TRACE("%d\n", _data[0][0]);

	for (auto id : INPUT_ID())
	{
		_moveFlg.try_emplace(id, false);
	}


	_input = std::make_shared<KeyState>();
	_input->SetUp(_id);
	_puyo = std::make_shared<puyo>();
}
