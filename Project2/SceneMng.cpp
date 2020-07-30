#include <DxLib.h>
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

SceneMng* SceneMng::_sInstance;

void SceneMng::Run(void)
{
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_dbgStartDraw();
		for (auto&& id : _stage)
		{
			id->UpDate();
		}

		Draw();
	}
}

void SceneMng::Draw(void)
{
	_dbgAddDraw();

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
	_dbgSetup(255,255,255);
	_dbgSetAlpha(255);
	return true;
}

void SceneMng::Init(void)
{
	_stage.emplace_back(std::make_unique<Stage>(Vector2(gameOffSet_.x, _frameSize.y / 2), Vector2(gameSize_.x, gameSize_.y)));
	_stage.emplace_back(std::make_unique<Stage>(Vector2(gameSize_.x * (_stage.size() + 1)+ gameOffSet_.x, _frameSize.y / 2), Vector2(gameSize_.x, gameSize_.y)));
}

SceneMng::SceneMng() :
	pyoSize_(64), pyoRadius_(pyoSize_ / 2), gameSize_(Vector2(pyoSize_ * 6, pyoSize_ * 13)), gameOffSet_(Vector2(66, 66)),
	_frameSize(Vector2(64, 66)), _screenSize(Vector2(gameSize_.x * 3 + _frameSize.x * 2, gameSize_.y + gameOffSet_.y))
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
