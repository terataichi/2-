#include <algorithm>
#include <DxLib.h>
#include "Stage.h"
#include "SceneMng.h"
#include "KeyState.h"
#include "PadState.h"

int Stage::playCnt = 1;

Stage::Stage()
{
	_stageID = MakeScreen(lpSceneMng._gameSize.x, lpSceneMng._gameSize.y);
}

Stage::Stage(Vector2&& offSet,Vector2&& size) :_offSet(std::move(offSet)), _size(std::move(size))
{
	Init();
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
	_puyo->UpDate();
	_input->UpDate();

	// ˆÚ“®×ÑÀÞ
	auto move = [](std::weak_ptr<InputState> keyData, INPUT_ID id, std::weak_ptr<puyo> pu,Vector2&& vec)
	{
		if (!keyData.expired() && !pu.expired())
		{
			if (keyData.lock()->state(id).first && !keyData.lock()->state(id).second)
			{
				pu.lock()->Move(vec);
			}
		}
	};
	move(_input, INPUT_ID::BUTTON_UP, _puyo, Vector2(0, -lpSceneMng._pyoSize));
	move(_input, INPUT_ID::BUTTON_LEFT, _puyo, Vector2(-lpSceneMng._pyoSize, 0));
	move(_input, INPUT_ID::BUTTON_RIGHT, _puyo, Vector2(lpSceneMng._pyoSize, 0));
	move(_input, INPUT_ID::BUTTON_DOWN, _puyo, Vector2(0, lpSceneMng._pyoSize));

	Draw();
}

void Stage::Init()
{
	_stageID = MakeScreen(_size.x, _size.y);
	_input = std::make_shared<PadState>(playCnt);
	_puyo = std::make_shared<puyo>();
	_id = playCnt;
	playCnt++;
}
