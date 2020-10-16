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
	TRACE("�^�C�g���V�[���̐���\n");
	screen_size_x_ = 0;
	screen_size_y_ = 0;

	auto ip = lpNetWork.GetIP();
	TRACE("%d.%d.%d.%d\n", ip.d1, ip.d2, ip.d3, ip.d4);

	func_[UpdateMode::SetHostIP] = std::bind(&TitleScene::SetHostIP, this);
	func_[UpdateMode::SetNetWork] = std::bind(&TitleScene::SetNetWorkMode, this);
	func_[UpdateMode::Play] = std::bind(&TitleScene::PlayUpdate, this);
	func_[UpdateMode::StartInit] = std::bind(&TitleScene::StartInit, this);

	updateMode_ = UpdateMode::SetNetWork;
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
	// �l�b�g���[�N�̃A�b�v�f�[�g
	func_[updateMode_]();
	if (!lpNetWork.Update())
	{
		// �ڑ����؂ꂽ���ɂ��ׂĂ���蒼��
		lpNetWork.CloseNetWork();
		updateMode_ = UpdateMode::SetNetWork;
	}
	return scene;
}

void TitleScene::Draw()
{
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("image03")[0],pos_.x,pos_.y,1,rad_,0 });
}

void TitleScene::PlayUpdate(void)
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
}

void TitleScene::SetNetWorkMode(void)
{
	int mode;			// ���[�h�I��p

	do {
		std::cout << "���[�h��I�����Ă��������B\n 0 : �z�X�g,\n 1 : �Q�X�g,\n 2 : �I�t���C��,\n" << std::endl;
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
			TRACE("���������͂���Ă܂���A������x���͂��Ă�������");
		}
	} while (mode < 0 || mode > 2);

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
}

void TitleScene::SetHostIP(void)
{
	TRACE("�ڑ����IP�A�h���X����͂��Ă�������");

	IPDATA hostIP;		// �z�X�g��IP
	std::string ip;
	std::cin >> ip;

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

	TRACE("%d.%d.%d.%d", hostIP.d1, hostIP.d2, hostIP.d3, hostIP.d4);

	if (lpNetWork.ConnectHost(hostIP))
	{
		updateMode_ = UpdateMode::StartInit;
	}
}

void TitleScene::StartInit(void)
{
	if (lpNetWork.GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (lpNetWork.CheckConnect() && lpNetWork.GetActive() == ActiveState::Init)
		{
			// ���������̑��M�����đҋ@
			TRACE("���������̑��M\n");
			lpNetWork.SendStanby();
		}
	}
	else if (lpNetWork.GetNetWorkMode() == NetWorkMode::GUEST)
	{
		if (lpNetWork.GetActive() == ActiveState::Init)
		{
			TRACE("���������̎󂯎��\n");

			// �X�^�[�g���̑��M
			lpNetWork.SendStart();
		}
	}
	if (lpNetWork.GetActive() == ActiveState::Play)
	{
		TRACE("�Q�[�����[�h�Ɉڍs���܂�\n");
		updateMode_ = UpdateMode::Play;
	}
}
