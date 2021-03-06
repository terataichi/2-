#include <algorithm>
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
	if (!lpEffectMng.Init(Vector2(screenSize_.x, screenSize_.y)))
	{
		DxLib_End();
		return;
	}

	activeScene_ = std::make_unique<TitleScene>();

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE) && end_)
	{
		_dbgStartDraw();

		activeScene_ = (*activeScene_).Update(std::move(activeScene_));			// 現在のシーンアップデート
		(*activeScene_).Draw();													// キューを投げる
		lpEffectMng.Update();

		Draw();																	// すべて描画
	}
	Destroy();
}

bool SceneMng::AddDrawQue(drawQueT que)
{
	drawList_.emplace_back(que);
	return true;
}

void SceneMng::SetEnd(void)
{
	end_ = false;
}

void SceneMng::Draw(void)
{
	_dbgAddDraw();

	// リストのソート
	std::sort(drawList_.begin(), drawList_.end(), [](drawQueT que1, drawQueT que2)
		{
			return (std::tie(std::get<static_cast<int>(DrawQue::ZOrder)>(que1))) >
				(std::tie(std::get<static_cast<int>(DrawQue::ZOrder)>(que2)));
		});

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	for (auto que : drawList_)
	{
		int id, x, y;
		double ex;
		float angle;

		std::tie(id, x, y, ex, angle, std::ignore) = que;

		DrawRotaGraph(x,y, ex, angle, id, true);
	}

	drawList_.clear();
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
	end_ = true;
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
	Init();
}

SceneMng::~SceneMng()
{
}
