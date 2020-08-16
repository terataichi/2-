#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "SceneMng.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../common/EffectMng.h"
#include "GameScene.h"
#include "TitleScene.h"

SceneMng* SceneMng::sInstance_;

void SceneMng::Run(void)
{
	lpEffectMng.Init(Vector2(screenSize_.x, screenSize_.y));

	activeScene_ = std::make_unique<TitleScene>();

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_dbgStartDraw();

		activeScene_ = (*activeScene_).Update(std::move(activeScene_));

		lpEffectMng.Update();
		Draw();
	}
}

void SceneMng::Draw(void)
{
	_dbgAddDraw();

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	(*activeScene_).Draw();

	ScreenFlip();
}

bool SceneMng::SysInit(void)
{
	ChangeWindowMode(true);
	SetWindowText("1916025_寺崎大智");
	SetGraphMode(screenSize_.x, screenSize_.y, 16);

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
}

SceneMng::SceneMng() :
	pyoSize_(64), pyoRadius_(pyoSize_ / 2), gameSize_(Vector2(pyoSize_ * 6, pyoSize_ * 13)), gameOffSet_(Vector2(66, 66)),
	_frameSize(Vector2(64, 66)), screenSize_(Vector2(gameSize_.x * 3 + _frameSize.x * 2, gameSize_.y + gameOffSet_.y))
{
	TRACE("シーンマネージャーの生成\n");
	if (!SysInit())
	{
		TRACE("DXLIB :失敗しました\n");
	}
	//Init();
}

SceneMng::~SceneMng()
{
}
