#include "TitleScene.h"
#include <DxLib.h>
#include "../NetWork/NetWork.h"

#include "../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	TRACE("タイトルシーンの生成");
	screen_size_x_ = 0;
	screen_size_y_ = 0;

	auto ip = lpNetWork.GetIP();
	TRACE("%d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(void)
{
	GetDrawScreenSize(&screen_size_x_, &screen_size_y_);
}

uniqueBase TitleScene::Update(uniqueBase scene)
{
	return scene;
}

void TitleScene::Draw()
{
}
