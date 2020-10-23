#include <algorithm>
#include <DxLib.h>
#include "SceneMng.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"


SceneMng* SceneMng::sInstance_;

void SceneMng::Run(void)
{

	activeScene_ = std::make_unique<TitleScene>();

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE) && end_)
	{
		_dbgStartDraw();

		activeScene_ = (*activeScene_).Update(std::move(activeScene_));			// 現在のシーンアップデート
		(*activeScene_).Draw();													// キューを投げる

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
	SetAlwaysRunFlag(true);
	_dbgSetup(255,255,255);
	_dbgSetAlpha(255);
	return true;
}

void SceneMng::Init(void)
{
	end_ = true;
}

SceneMng::SceneMng() : screenSize_(32 * 21,32 * 17)
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
