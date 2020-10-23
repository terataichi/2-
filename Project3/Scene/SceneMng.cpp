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

		activeScene_ = (*activeScene_).Update(std::move(activeScene_));			// ���݂̃V�[���A�b�v�f�[�g
		(*activeScene_).Draw();													// �L���[�𓊂���

		Draw();																	// ���ׂĕ`��
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

	// ���X�g�̃\�[�g
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
	SetWindowText("1916025_�����q");
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
	TRACE("�V�[���}�l�[�W���[�̐���\n");
	if (!SysInit())
	{
		TRACE("DXLIB :���s���܂���\n");
	}
	Init();
}

SceneMng::~SceneMng()
{
}
