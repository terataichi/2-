#include "LoginScene.h"
#include <DxLib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../NetWork/NetWork.h"
#include "../common/TileMap.h"
#include "../common/ImageMng.h"
#include "../Scene/SceneMng.h"
#include "../Input/PadState.h"
#include "../Input/INPUT_ID.h"
#include "GameScene.h"
#include "CrossOverScene.h"

//#include "../TMXParser-master/include/TMXParser.h"

#include "../_debug/_DebugConOut.h"

LoginScene::LoginScene()
{
	TRACE("タイトルシーンの生成\n");
	screen_size_x_ = 0;
	screen_size_y_ = 0;

	auto address = lpNetWork.GetMyIP();

	for (auto ip : address)
	{
		TRACE("%d.%d.%d.%d\n", ip.d1, ip.d2, ip.d3, ip.d4);
	}

	func_[UpdateMode::SetHostIP] = std::bind(&LoginScene::SetHostIP, this);
	func_[UpdateMode::SetNetWork] = std::bind(&LoginScene::SetNetWorkMode, this);
	func_[UpdateMode::Play] = std::bind(&LoginScene::PlayUpdate, this);
	func_[UpdateMode::StartInit] = std::bind(&LoginScene::StartInit, this);;

	if (!tileMap_.LoadTmx("TileMap/Stage01.tmx"))
	{
		TRACE("ファイルの読み込みに失敗\n");
		return;
	}


	updateMode_ = UpdateMode::SetNetWork;
	Init();
}

LoginScene::~LoginScene()
{
}

void LoginScene::Init(void)
{
	GetDrawScreenSize(&screen_size_x_, &screen_size_y_);
	input_ = std::make_unique<PadState>();
	input_->SetUp(0);
	wasHost_ = false;
	pos_ = { 100,100 };
	speed_ = 10;
	rad_ = 0;
	wasHost_ = false;
	//tileMap_.SendTmxData();
	DrawOwnScene();
}

uniqueBase LoginScene::Update(uniqueBase scene)
{
	// ネットワークのアップデート
	if (!func_[updateMode_]() || lpNetWork.GetNetWorkMode() == NetWorkMode::OFFLINE)
	{
		return std::make_unique<CrossOverScene>(std::move(scene), std::make_unique<GameScene>());
	}
	DrawOwnScene();
	return scene;
}

void LoginScene::DrawOwnScene()
{
	SetDrawScreen(drawScreen_);
	DrawBox(100, 100, 200, 200, 0xfffff, true);
}

bool LoginScene::PlayUpdate(void)
{
	(*input_)();

	auto move = [&](INPUT_ID id, Vector2 speed_)
	{
		if (input_->GetTrgData().at(id)[static_cast<int>(Trg::Now)])
		{
			pos_ += speed_;
		}
	};

	move(INPUT_ID::BUTTON_LEFT, { -speed_,0 });
	move(INPUT_ID::BUTTON_RIGHT, { speed_ ,0 });
	move(INPUT_ID::BUTTON_UP, { 0, -speed_ });
	move(INPUT_ID::BUTTON_DOWN, { 0, speed_ });
	return true;
}

bool LoginScene::SetNetWorkMode(void)
{
	int mode;												// モード選択用

	// 前回のホストのIPアドレス取得
	std::ifstream ifs("hostIp.txt");
	std::string str;

	bool fFlg = true;										// ファイルが開けているか						
	if (ifs.fail())
	{
		str = "\n";
		fFlg = false;
	}
	else
	{
		std::string ip;
		getline(ifs, ip);
		str = " 2 : 前回のホストに接続。前回のIPアドレス : " + ip + ", \n";
	}

	do {
		std::cout << "モードを選択してください。\n 0 : ホスト,\n 1 : ゲスト,\n" << str <<" 3 : オフライン\n" << std::endl;
		std::cin >> mode;

		if (mode == 0)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::HOST);
		}
		else if (mode == 1)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::GUEST);
		}
		else if (mode == 2)
		{
			if (!fFlg)
			{
				TRACE("正しく入力されてません、もう一度入力してください");
			}
			else
			{
				wasHost_ = true;
				lpNetWork.SetNetWorkMode(NetWorkMode::GUEST);
			}
		}
		else if (mode == 3)
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::OFFLINE);
		}
		else
		{
			TRACE("正しく入力されてません、もう一度入力してください");
		}
	} while (mode < 0 || mode > 3);

	// 結果発表
	switch (lpNetWork.GetNetWorkMode())
	{
	case NetWorkMode::OFFLINE:
		TRACE("オフラインです");
		updateMode_ = UpdateMode::Play;
			break;
	case NetWorkMode::HOST:
		TRACE("ホストです");
		updateMode_ = UpdateMode::StartInit;
			break;
	case NetWorkMode::GUEST:
		TRACE("ゲストです");
		updateMode_ = UpdateMode::SetHostIP;
			break;
	default:
		TRACE("エラー")
			break;
	}
	TRACE("状態 : %dです\n", lpNetWork.GetActive());

	return true;
}

bool LoginScene::SetHostIP(void)
{

	IPDATA hostIP;		// ホストのIP
	std::string ip;

	auto readHostIP = [&]()
	{
		// 前回のホストのIPアドレス取得
		std::ifstream ifs("hostIp.txt");
		if (ifs.fail())
		{
			TRACE("ファイルの読み込みに失敗しました。\n");
			wasHost_ = false;
			return false;
		}
		// ファイルの内容を格納
		getline(ifs, ip);
		ifs.close();
		return true;
	};

	if(wasHost_)
	{
		if (!readHostIP())
		{
			TRACE("接続先のIPアドレスを入力してください\n");
			std::cin >> ip;
		}
	}
	else
	{
		TRACE("接続先のIPアドレスを入力してください\n");
		std::cin >> ip;
	}

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

	//TRACE("%d.%d.%d.%d", hostIP.d1, hostIP.d2, hostIP.d3, hostIP.d4);

	if (lpNetWork.ConnectHost(hostIP))
	{
		// ファイルへの書き込み
		// なかったら新しくファイルを作ってくれる
		// std::ios::trunc：今回はファイルを上書きしてかまわないので毎回新しくファイルを作り直す
		std::ofstream ofs("hostIp.txt",std::ios::trunc);
		ofs << ip;
		ofs.close();

		updateMode_ = UpdateMode::StartInit;
	}

	return true;
}

bool LoginScene::StartInit(void)
{
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (lpNetWork.CheckConnect() && lpNetWork.GetActive() == ActiveState::Init)
		{
			// 初期化情報の送信をして待機
			TRACE("初期化情報の送信\n");
			tileMap_.SendTmxSizeData();
			tileMap_.SendTmxData();
			lpNetWork.SendStanby();
		}
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			return false;
		}
	}
	else if (lpNetWork.GetNetWorkMode() == NetWorkMode::GUEST)
	{
		if (lpNetWork.GetActive() == ActiveState::Init)
		{
			//TRACE("初期化情報の受け取り\n");
		}
	}
	if (lpNetWork.GetActive() == ActiveState::Play)
	{
		// スタート情報の送信
		lpNetWork.SendStart();
		TRACE("ゲームモードに移行します\n");
		updateMode_ = UpdateMode::Play;
		return false;
	}
	return true;
}
