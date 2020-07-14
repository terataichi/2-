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

	_dataBase.resize(STAGE_Y * STAGE_X);							// �S�̂̃T�C�Y�����

	//_data.resize(STAGE_Y);										// Y�̃T�C�Y���m�ۂ��Ă�����X���i�[���Ă���
	//for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	//{
	//	_data[j] = &_dataBase[j * STAGE_X];
	//}

	_dataBase[0] = 500;
	for (int j = 0; j < static_cast<int>(STAGE_Y); j++)
	{
		_data.emplace_back(&_dataBase[j * STAGE_X]);
	}
	TRACE("%d\n", _data[0][0]);

	for (auto id : INPUT_ID())
	{
		_moveFlg.try_emplace(id, false);
	}


	_input = std::make_shared<MouseState>();
	_input->SetUp(_id);
	_puyo = std::make_shared<puyo>();
}
