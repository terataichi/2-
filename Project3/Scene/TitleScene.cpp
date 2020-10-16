#include "TitleScene.h"
#include <DxLib.h>
#include <iostream>
#include <sstream>
#include "../NetWork/NetWork.h"
#include "../common/ImageMng.h"
#include "../Scene/SceneMng.h"
#include "../Input/PadState.h"
#include "../Input/INPUT_ID.h"

#include "../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	TRACE("タイトルシーンの生成\n");
	screen_size_x_ = 0;
	screen_size_y_ = 0;

	auto ip = lpNetWork.GetIP();
	TRACE("%d.%d.%d.%d\n", ip.d1, ip.d2, ip.d3, ip.d4);

	func_[UpdateMode::SetHostIP] = std::bind(&TitleScene::SetHostIP, this);
	func_[UpdateMode::SetNetWork] = std::bind(&TitleScene::SetNetWorkMode, this);
	func_[UpdateMode::Play] = std::bind(&TitleScene::PlayUpdate, this);
	func_[UpdateMode::StartInit] = std::bind(&TitleScene::StartInit, this);

	updateMode_ = UpdateMode::SetNetWork;

	TRACE("状態 : %dです\n", lpNetWork.GetActive());
	Init();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(void)
{
	GetDrawScreenSize(&screen_size_x_, &screen_size_y_);
	input_ = std::make_unique<PadState>();
	input_->SetUp(0);

	pos_ = { 100,100 };
	speed_ = 10;
	rad_ = 0;
}

uniqueBase TitleScene::Update(uniqueBase scene)
{
	(*input_)();

	// ネットワークのアップデート
	func_[updateMode_]();

	auto move = [&](INPUT_ID id, Vector2 speed_)
	{
		if (input_->GetTrgData().at(id)[static_cast<int>(Trg::Now)])
		{
			pos_ += speed_;
		}
	};

	move(INPUT_ID::BUTTON_LEFT, { -speed_,0 });
	move(INPUT_ID::BUTTON_RIGHT, { speed_ ,0});
	move(INPUT_ID::BUTTON_UP, { 0, -speed_ });
	move(INPUT_ID::BUTTON_DOWN, { 0, speed_ });

	lpNetWork.SendMes(pos_);
	lpNetWork.Update();
	lpNetWork.GetReceiveData(pos_);
	return scene;
}

void TitleScene::Draw()
{
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("image03")[0],pos_.x,pos_.y,1,rad_,0 });
}

void TitleScene::PlayUpdate(void)
{
}

void TitleScene::SetNetWorkMode(void)
{
	int mode;			// モード選択用

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
		}
		else
		{
			TRACE("正しく入力されてません、もう一度入力してください");
		}
	} while (mode < 0 || mode > 2);

	// 結果発表
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
}

void TitleScene::SetHostIP(void)
{
	IPDATA hostIP;		// ホストのIP
	std::string ip;
	std::cin >> ip;

	// ipに入力された情報をHostIPに格納
	std::istringstream iss{ ip };
	std::string data;

	auto GetIPNum = [&]()
	{
		getline(iss, data, '.');
		return atoi(data.c_str());
	};
	hostIP.d1 = GetIPNum();
	hostIP.d2 = GetIPNum();
	hostIP.d3 = GetIPNum();
	hostIP.d4 = GetIPNum();

	TRACE("%d.%d.%d.%d", hostIP.d1, hostIP.d2, hostIP.d3, hostIP.d4);

	lpNetWork.ConnectHost(hostIP);
}

void TitleScene::StartInit(void)
{

}
