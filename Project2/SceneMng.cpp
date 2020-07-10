#include <DxLib.h>
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"

SceneMng* SceneMng::_sInstance;

void SceneMng::Run(void)
{
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		for (auto&& id : _stage)
		{
			id->UpDate();
		}

		Draw();
	}
}

void SceneMng::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	for (auto&& id : _stage)
	{
		DrawGraph(id->offSet().x, id->offSet().y, id->GetStageID(), true);
	}

	ScreenFlip();
}

bool SceneMng::SysInit(void)
{
	ChangeWindowMode(true);
	SetWindowText("1916025_寺崎大智");
	SetGraphMode(_screenSize.x, _screenSize.y, 16);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	return true;
}

void SceneMng::Init(void)
{
	_stage.emplace_back(std::make_unique<Stage>(Vector2(_gameOffSet.x, _frameSize.y / 2), Vector2(_gameSize.x, _gameSize.y)));
	_stage.emplace_back(std::make_unique<Stage>(Vector2(_gameSize.x * 2 + _gameOffSet.x, _frameSize.y / 2), Vector2(_gameSize.x, _gameSize.y)));
}

SceneMng::SceneMng() :
	_pyoSize(64), _pyoRadius(_pyoSize / 2), _gameSize(Vector2(_pyoSize * 6, _pyoSize * 13)), _gameOffSet(Vector2(66, 66)),
	_frameSize(Vector2(64, 66)), _screenSize(Vector2(_gameSize.x * 3 + _frameSize.x * 2, _gameSize.y + _gameOffSet.y))
{
	TRACE("シーンマネージャーの生成\n");
	if (!SysInit())
	{
		TRACE("DXLIB :失敗しました\n");
	}
	Init();
}

SceneMng::~SceneMng()
{
}
