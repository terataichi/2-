#include "LoginScene.h"
#include <DxLib.h>
#include <iostream>
#include <sstream>
#include <fstream>
//#include "../NetWork/NetWork.h"
#include "../common/TileMap.h"
#include "../common/ImageMng.h"
#include "../Scene/SceneMng.h"
#include "../Input/PadState.h"
#include "../Input/INPUT_ID.h"
#include "../common/TileMap.h"
#include "GameScene.h"
#include "CrossOverScene.h"

//#include "../TMXParser-master/include/TMXParser.h"

#include "../_debug/_DebugConOut.h"

LoginScene::LoginScene()
{
	TRACE("�^�C�g���V�[���̐���\n");
	screen_size_x_ = 0;
	screen_size_y_ = 0;

	myIP_ = lpNetWork.GetMyIP();

	for (auto ip : myIP_)
	{
		if (ip.d1 != 0)
		{
			TRACE("%d.%d.%d.%d\n", ip.d1, ip.d2, ip.d3, ip.d4);
		}
	}

	funcUpdate_[UpdateMode::SetLastHostIP] = std::bind(&LoginScene::SetLastHostIP, this);
	funcUpdate_[UpdateMode::SelectHostIP] = std::bind(&LoginScene::SelectHostIP, this);
	funcUpdate_[UpdateMode::SetNetWork] = std::bind(&LoginScene::SetNetWorkMode, this);
	funcUpdate_[UpdateMode::Play] = std::bind(&LoginScene::PlayUpdate, this);
	funcUpdate_[UpdateMode::StartInit] = std::bind(&LoginScene::StartInit, this);;

	netWorkModeMaxSize_ = static_cast<int>(NetWorkMode::MAX);
	netWorkModeTarget_ = 0;

	tileMap_ = std::make_shared<TileMap>();
	if (!tileMap_->LoadTmx("TileMap/Stage01.tmx"))
	{
		TRACE("�t�@�C���̓ǂݍ��݂Ɏ��s\n");
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
	selectIpTargetNum_ = 0;
	selectIpTarget_ = { 0 ,0 };
	// tileMap_.SendTmxData();

	targetNum_[0] = "n";
	targetNum_[1] = "0";
	targetNum_[2] = ".";
	targetNum_[3] = "1";
	targetNum_[4] = "2";
	targetNum_[5] = "3";
	targetNum_[6] = "4";
	targetNum_[7] = "5";
	targetNum_[8] = "6";
	targetNum_[9] = "7";
	targetNum_[10] = "8";
	targetNum_[11] = "9";


	InitFunc();
	DrawOwnScene();
}

uniqueBase LoginScene::Update(uniqueBase scene)
{
	(*input_)();
	// �l�b�g���[�N�̃A�b�v�f�[�g
	if (!funcUpdate_[updateMode_]())
	{
		return std::make_unique<CrossOverScene>(std::move(scene), std::make_unique<GameScene>());
	}
	DrawOwnScene();
	return scene;
}

void LoginScene::DrawOwnScene()
{
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();

	for (auto ip : myIP_)
	{
		if (ip.d1 != 0)
		{			
			DrawFormatString(0,30,0xffffff,"%d.%d.%d.%d\n", ip.d1, ip.d2, ip.d3, ip.d4);
		}
	}

	// �e�A�b�v�f�[�g�̕`��
	funcDraw_[updateMode_]();

	auto size = lpSceneMng.screenSize_ / 2;
	if (lpNetWork.GetCountDownFlg())
	{
		auto startTime = lpNetWork.GetStartTime();

		chronoTime now = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

		DrawFormatString(size.x, size.y, 0xffffff, "�c��F%d�b", (COUNT_DOWN_MAX - time) / 1000);
	}
	else
	{
		DrawFormatString(size.x, size.y, 0xffffff, "�ҋ@��");
	}
}

void LoginScene::InitFunc(void)
{
	auto play = [&]() {};

	auto startInit = [&]() 
	{
	
	};
	
	funcDraw_[UpdateMode::SetLastHostIP] = std::bind(&LoginScene::DrawSetHostIP, this);
	funcDraw_[UpdateMode::SelectHostIP] = std::bind(&LoginScene::DrawSetHostIP, this);
	funcDraw_[UpdateMode::SetNetWork] = std::bind(&LoginScene::DrawSetNetWork, this);
	funcDraw_.try_emplace(UpdateMode::Play, play);
	funcDraw_.try_emplace(UpdateMode::StartInit, startInit);
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

	// �O��̃z�X�g��IP�A�h���X�擾
	std::ifstream ifs("init/hostIp.txt");

	bool fFlg = true;										// �t�@�C�����J���Ă��邩						
	if (ifs.fail())
	{
		fFlg = false;
		netWorkModeMaxSize_ = 2;
	}

	int mode = SelectNetWorkMode();							// ���[�h�I��p
	// �����蔻��
	//std::cin >> mode;

	if (mode == 0)
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::HOST);
	}
	else if (mode == 1)
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::GUEST);
	}
	else if (mode == 3)
	{
		if (!fFlg)
		{
			TRACE("�t�@�C�����J���܂���ł���\n");
		}
		else
		{
			wasHost_ = true;
		}
		lpNetWork.SetNetWorkMode(NetWorkMode::GUEST);
	}
	else if (mode == 2)
	{
		lpNetWork.SetNetWorkMode(NetWorkMode::OFFLINE);
		return false;
	}
	else
	{
		return true;
	}

	// ���ʔ��\
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::OFFLINE)
	{
		TRACE("�I�t���C���ł�");
		updateMode_ = UpdateMode::Play;
	}
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::HOST)
	{
		TRACE("�z�X�g�ł�");
		updateMode_ = UpdateMode::StartInit;
	}
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::GUEST)
	{
		TRACE("�Q�X�g�ł�");
		updateMode_ = UpdateMode::SelectHostIP;

		if (wasHost_)
		{
			updateMode_ = UpdateMode::SetLastHostIP;
		}
	}
	TRACE("��� : %d�ł�\n", lpNetWork.GetActive());

	return true;
}

int LoginScene::SelectNetWorkMode(void)
{
	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_DOWN))
	{
		netWorkModeTarget_++;
	}

	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_UP))
	{
		netWorkModeTarget_--;
	}

	if (netWorkModeTarget_ < 0)
	{
		netWorkModeTarget_ = netWorkModeMaxSize_;
	}
	if (netWorkModeTarget_ > netWorkModeMaxSize_)
	{
		netWorkModeTarget_ = 0;
	}

	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_ATTACK))
	{
		return netWorkModeTarget_;
	}

	return -1;
}

void LoginScene::DrawSetNetWork(void)
{
	// �O��̃z�X�g��IP�A�h���X�擾
	std::ifstream ifs("init/hostIp.txt");

	DrawBox(0, 0, lpSceneMng.screenSize_.x, lpSceneMng.screenSize_.y, 0xffffff, true);

	int x = 200;
	if (!ifs.fail())
	{
		std::string ip;
		getline(ifs, ip);
		DrawRotaGraph(x + 50, 400, 1, 0, lpImageMng.GetHandle("lasthost")[0], true);
	}
	DrawRotaGraph(x, 100, 1, 0, lpImageMng.GetHandle("host")[0], true);
	DrawRotaGraph(x, 200, 1, 0, lpImageMng.GetHandle("guest")[0], true);
	DrawRotaGraph(x, 300, 1, 0, lpImageMng.GetHandle("offline")[0], true);

	DrawRotaGraph(50, 100 + netWorkModeTarget_ * 100, 1, 0, lpImageMng.GetHandle("ten")[0], true);

	//std::string a = "���[�h��I�����Ă��������B\n 0 : �z�X�g,\n 1 : �Q�X�g,\n" + str + " 3 : �I�t���C��\n";
	//DrawFormatString(100, 100, 0xfffff, a.c_str());
	// std::cout << "���[�h��I�����Ă��������B\n 0 : �z�X�g,\n 1 : �Q�X�g,\n" << str << " 3 : �I�t���C��\n" << std::endl;
}

bool LoginScene::SetLastHostIP(void)
{
	auto readHostIP = [&]()
	{
		// �O��̃z�X�g��IP�A�h���X�擾
		std::ifstream ifs("init/hostIp.txt");
		if (ifs.fail())
		{
			TRACE("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n");
			return false;
		}
		// �t�@�C���̓��e���i�[
		getline(ifs, hostIPAddress_);
		return true;
	};

	if(!readHostIP())
	{
		TRACE("�V�������͂��Ă�������\n");
		updateMode_ = UpdateMode::SelectHostIP;
		return true;
	}
	SetHostIP(hostIPAddress_);
	return true;
}

bool LoginScene::SetHostIP(std::string hostIPAddress)
{
	IPDATA hostIP;		// �z�X�g��IP
	// ip�ɓ��͂��ꂽ����HostIP�Ɋi�[
	std::istringstream iss{ hostIPAddress };
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
		// �t�@�C���ւ̏�������
		// �Ȃ�������V�����t�@�C��������Ă����
		// std::ios::trunc�F����̓t�@�C�����㏑�����Ă��܂�Ȃ��̂Ŗ���V�����t�@�C������蒼��
		std::ofstream ofs("init/hostIp.txt", std::ios::trunc);
		ofs << hostIPAddress_;

		updateMode_ = UpdateMode::StartInit;
	}
	return true;
}

bool LoginScene::SelectHostIP(void)
{
	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_DOWN))
	{
		if (selectIpTargetNum_ - SELECT_IP_MAX_X >= 0)
		{
			selectIpTargetNum_ -= SELECT_IP_MAX_X;
			selectIpTarget_.y--;
		}
	}
	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_UP))
	{
		if (selectIpTargetNum_ + SELECT_IP_MAX_X < SELECT_IP_MAX_Y * SELECT_IP_MAX_X)
		{
			selectIpTargetNum_ += SELECT_IP_MAX_X;
			selectIpTarget_.y++;
		}
	}

	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_RIGHT))
	{
		if (selectIpTargetNum_ + 1 <= (selectIpTarget_.y * SELECT_IP_MAX_X) + SELECT_IP_MAX_X - 1)
		{
			selectIpTargetNum_++;
			selectIpTarget_.x++;
		}
	}
	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_LEFT))
	{
		if (selectIpTargetNum_ - 1 >= selectIpTarget_.y * SELECT_IP_MAX_X)
		{
			selectIpTargetNum_--;
			selectIpTarget_.x--;
		}
	}

	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_ATTACK))
	{
		int target = selectIpTargetNum_;
		if (targetNum_[target] == "n")
		{
			hostIPAddress_.clear();
			return true;
		}
		hostIPAddress_ += targetNum_[target];
	}

	if (input_->GetTrgOnePush(INPUT_ID::BUTTON_ROTA_R))
	{
		SetHostIP(hostIPAddress_);
	}

	return true;
}

void LoginScene::DrawSetHostIP(void)
{
	DrawBox(0, 0, lpSceneMng.screenSize_.x, lpSceneMng.screenSize_.y, 0xffffff, true);

	Vector2 offset{ 250,150 };
	Vector2 size{ 50,61 };
	Vector2 targetPos{ size.x * selectIpTarget_.x , size.y * selectIpTarget_.y };
	DrawGraph(offset.x, offset.y,lpImageMng.GetHandle("ipnum")[0], true);
	DrawBox(offset.x + targetPos.x,
			offset.y + (size.y * 3) - targetPos.y,
			offset.x + size.x + targetPos.x,
			offset.y + (size.y * 3) + size.y - targetPos.y,
			0xf, false);
	selectIpTargetNum_;
}

bool LoginScene::StartInit(void)
{
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (lpNetWork.GetActive() == ActiveState::Init)
		{
			// ���������̑��M�����đҋ@
			TRACE("���������̑��M\n");
			tileMap_->SendTmxData();
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
			//TRACE("���������̎󂯎��\n");
		}
		if (lpNetWork.GetActive() == ActiveState::Play)
		{
			// �X�^�[�g���̑��M
			lpNetWork.SendStart();
			TRACE("�Q�[�����[�h�Ɉڍs���܂�\n");
			updateMode_ = UpdateMode::Play;
			return false;
		}
	}

	return true;
}

