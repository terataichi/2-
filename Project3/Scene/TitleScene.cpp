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


	int mode;			// ���[�h�I��p
	IPDATA hostIP;		// �ق��Ƃ�PI

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

			lpNetWork.ConnectHost(hostIP);
		}
		else
		{
			TRACE("���������͂���Ă܂���");
		}
	} while (mode < 0 || mode > 2);

	 
	switch (lpNetWork.GetNetWorkMode())
	{
	case NetWorkMode::OFFLINE:
		TRACE("�I�t���C���ł�")
		break;
	case NetWorkMode::HOST:
		TRACE("�z�X�g�ł�")
		break;
	case NetWorkMode::GUEST:
		TRACE("�Q�X�g�ł�")
		break;
	default:
		TRACE("�G���[")
		break;
	}

	TRACE("��� : %d�ł�\n", lpNetWork.GetActive());
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

	lpNetWork.SetSendData(pos_);
	lpNetWork.Update();
	lpNetWork.GetReceiveData(pos_);
	return scene;
}

void TitleScene::Draw()
{
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("image03")[0],pos_.x,pos_.y,1,rad_,0 });
}
