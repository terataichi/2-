#include <algorithm>
#include <DxLib.h>
#include "SceneMng.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"


SceneMng* SceneMng::sInstance_;

void SceneMng::Run(void)
{

	activeScene_ = std::make_unique<LoginScene>();

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE) && end_)
	{
		_dbgStartDraw();

		activeScene_ = (*activeScene_).Update(std::move(activeScene_));			// ���݂̃V�[���A�b�v�f�[�g

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

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	(*activeScene_).Draw();
	_dbgAddDraw();
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
	_dbgSetup(screenSize_.x,screenSize_.y,255);
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
