#include <algorithm>
#include <DxLib.h>
#include "Stage.h"
#include "SceneMng.h"
#include "KeyState.h"
#include "PadState.h"

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
	for (auto data : _input->GetTrgData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			_puyo->Move(data.first);
		}
	}
	_puyo->UpDate();


	Draw();
}

void Stage::Init()
{
	_stageID = MakeScreen(_size.x, _size.y);
	_input = std::make_shared<PadState>();
	_input->SetUp(_id);
	_puyo = std::make_shared<puyo>();
}
