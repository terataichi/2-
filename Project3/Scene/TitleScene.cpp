#include "TitleScene.h"
#include <DxLib.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "../NetWork/NetWork.h"

#include "../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	TRACE("タイトルシーンの生成");
	screen_size_x_ = 0;
	screen_size_y_ = 0;

	auto ip = lpNetWork.GetIP();
	TRACE("%d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);


	int mode;			// モード選択用
	IPDATA hostIP;		// ほすとのPI

	do {
		std::cout << "モードを選択してください。\n 0 : ホスト,\n 1 : ゲスト,\n 2 : オフライン,\n" << std::endl;
		std::cin >> mode;

		if (mode == 2)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::OFFLINE);
		}
		else if (mode == 0)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::HOST);
		}
		else if (mode == 1)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::GUEST);
			std::string ip;
			std::cin >> ip;

			// ipに入力された情報をHostIPに格納
			std::istringstream iss(ip);
			std::string a;
			std::getline(iss, a,'.');
			hostIP.d1 = atoi(a.c_str());


			lpNetWork.ConnectHost(hostIP);
		}
		else
		{
			TRACE("正しく入力されてません");
		}
	} while (mode < 0 || mode > 2);

	 
	switch (lpNetWork.GetNetWorkMode())
	{
	case NetWorkMode::OFFLINE:
		TRACE("オフラインです")
		break;
	case NetWorkMode::HOST:
		TRACE("ホストです")
		break;
	case NetWorkMode::GUEST:
		TRACE("ゲストです")
		break;
	default:
		TRACE("エラー")
		break;
	}

	TRACE("状態 : %dです\n", lpNetWork.GetActive());

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
