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
	TRACE("�^�C�g���V�[���̐���\n");
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
	wasHost_ = false;
	//tileMap_.SendTmxData();
	DrawOwnScene();
}

uniqueBase LoginScene::Update(uniqueBase scene)
{
	// �l�b�g���[�N�̃A�b�v�f�[�g
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
	int mode;												// ���[�h�I��p

	// �O��̃z�X�g��IP�A�h���X�擾
	std::ifstream ifs("hostIp.txt");
	std::string str;

	bool fFlg = true;										// �t�@�C�����J���Ă��邩						
	if (ifs.fail())
	{
		str = "\n";
		fFlg = false;
	}
	else
	{
		std::string ip;
		getline(ifs, ip);
		str = " 2 : �O��̃z�X�g�ɐڑ��B�O���IP�A�h���X : " + ip + ", \n";
	}

	do {
		std::cout << "���[�h��I�����Ă��������B\n 0 : �z�X�g,\n 1 : �Q�X�g,\n" << str <<" 3 : �I�t���C��\n" << std::endl;
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
				TRACE("���������͂���Ă܂���A������x���͂��Ă�������");
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
			TRACE("���������͂���Ă܂���A������x���͂��Ă�������");
		}
	} while (mode < 0 || mode > 3);

	// ���ʔ��\
	switch (lpNetWork.GetNetWorkMode())
	{
	case NetWorkMode::OFFLINE:
		TRACE("�I�t���C���ł�");
		updateMode_ = UpdateMode::Play;
			break;
	case NetWorkMode::HOST:
		TRACE("�z�X�g�ł�");
		updateMode_ = UpdateMode::StartInit;
			break;
	case NetWorkMode::GUEST:
		TRACE("�Q�X�g�ł�");
		updateMode_ = UpdateMode::SetHostIP;
			break;
	default:
		TRACE("�G���[")
			break;
	}
	TRACE("��� : %d�ł�\n", lpNetWork.GetActive());

	return true;
}

bool LoginScene::SetHostIP(void)
{

	IPDATA hostIP;		// �z�X�g��IP
	std::string ip;

	auto readHostIP = [&]()
	{
		// �O��̃z�X�g��IP�A�h���X�擾
		std::ifstream ifs("hostIp.txt");
		if (ifs.fail())
		{
			TRACE("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n");
			wasHost_ = false;
			return false;
		}
		// �t�@�C���̓��e���i�[
		getline(ifs, ip);
		ifs.close();
		return true;
	};

	if(wasHost_)
	{
		if (!readHostIP())
		{
			TRACE("�ڑ����IP�A�h���X����͂��Ă�������\n");
			std::cin >> ip;
		}
	}
	else
	{
		TRACE("�ڑ����IP�A�h���X����͂��Ă�������\n");
		std::cin >> ip;
	}

	// ip�ɓ��͂��ꂽ����HostIP�Ɋi�[
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
		// �t�@�C���ւ̏�������
		// �Ȃ�������V�����t�@�C��������Ă����
		// std::ios::trunc�F����̓t�@�C�����㏑�����Ă��܂�Ȃ��̂Ŗ���V�����t�@�C������蒼��
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
			// ���������̑��M�����đҋ@
			TRACE("���������̑��M\n");
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
			//TRACE("���������̎󂯎��\n");
		}
	}
	if (lpNetWork.GetActive() == ActiveState::Play)
	{
		// �X�^�[�g���̑��M
		lpNetWork.SendStart();
		TRACE("�Q�[�����[�h�Ɉڍs���܂�\n");
		updateMode_ = UpdateMode::Play;
		return false;
	}
	return true;
}