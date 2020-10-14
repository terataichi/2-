#include "TitleScene.h"
#include <DxLib.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "../NetWork/NetWork.h"

#include "../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	TRACE("�^�C�g���V�[���̐���");
	screen_size_x_ = 0;
	screen_size_y_ = 0;

	auto ip = lpNetWork.GetIP();
	TRACE("%d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);


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
			std::istringstream iss(ip);
			std::string a;
			std::getline(iss, a,'.');
			hostIP.d1 = atoi(a.c_str());


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
