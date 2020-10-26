#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include <DxLib.h>
#include <fstream>
#include <iostream>

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::sInstance_(new NetWork);

ArrayIP NetWork::GetMyIP()
{
	ArrayIP ip;
	GetMyIPAddress(&ip[0],ip.size());
	return ip;
}

bool NetWork::SetNetWorkMode(NetWorkMode mode)
{
	switch (mode)
	{
	case NetWorkMode::OFFLINE:
		state_ = std::make_unique<NetWorkState>();
		break;
	case NetWorkMode::HOST:
		state_ = std::make_unique<HostState>();
		break;
	case NetWorkMode::GUEST:
		state_ = std::make_unique<GuestState>();
		break;
	default:
		TRACE("��O�F�I�t���C���ɐݒ肵�܂��B\n");
		state_ = std::make_unique<NetWorkState>();
		return false;
		break;
	}
	return false;
}

NetWorkMode NetWork::GetNetWorkMode(void)
{
	return state_->GetMode();
}

bool NetWork::CheckConnect(void)
{
	return state_->CheckConnect();
}

bool NetWork::Update(void)
{
	if (GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (state_->GetActive() == ActiveState::Stanby)
		{
			if (GetNetWorkDataLength(state_->GetNetHandle()) >= sizeof(MesData))
			{
				MesData data;
				NetWorkRecv(state_->GetNetHandle(), &data, sizeof(MesData));

				// �Q�[���X�^�[�g����M��
				if (data.type == MesType::GAME_START)
				{
					TRACE("�Q�X�g����ʒm���m�F�A�Q�[�����J�n���܂�\n");
					state_->SetActive(ActiveState::Play);
					return true;
				}
			}
		}
	}
	else if (GetNetWorkMode() == NetWorkMode::GUEST)
	{
		if (GetNetWorkDataLength(state_->GetNetHandle()) >= sizeof(MesData))
		{
			MesData data;
			NetWorkRecv(state_->GetNetHandle(), &data, sizeof(MesData));

			if (data.type == MesType::STANBY)
			{

				std::fstream fs("tmxData.tmx", std::ios::out);

				if (!fs)
				{
					TRACE("�������ݗp�̃t�@�C�����J���܂���\n");
					return false;
				}

				// �f�[�^�̎󂯎��
				for (auto data : RevBox)
				{
					char* ch = reinterpret_cast<char*>(&data);
					for (int i = 0; i < sizeof(data); i++)
					{
						if (ch[i] != 0)
						{
							fs << ch[i];
						}
					}
				}
				fs.close();

				TRACE("���������̊m�F�A�Q�[�����J�n�̍��}�����܂�\n");
				recvStanby_ = true;
				state_->SetActive(ActiveState::Play);
				return true;
			}
			if (data.type == MesType::TMX_SIZE)
			{
				tmxSize_ = data.data[0];
				RevBox.resize(tmxSize_);
				TRACE("TMX�f�[�^�T�C�Y�́F%d\n",data.data[0]);
				return true;
			}
			if (data.type == MesType::TMX_DATA)
			{
				RevBox[data.data[0]] = data.data[1];

				char tmp = data.data[1];
				tmp_++;

				TRACE("%c", RevBox[data.data[0]]);
				return true;
			}
		}
	}

	if (!state_->Update())
	{
		// �g�������̂����Z�b�g����
		recvStanby_ = false;
		return false;
	}
	return true;
}

bool NetWork::CloseNetWork(void)
{
	DxLib::CloseNetWork(state_->GetNetHandle());
	return true;
}

ActiveState NetWork::GetActive(void)
{
	return state_->GetActive();
}

void NetWork::SendStanby(void)
{
	// ���������̑��M
	MesData tmpData;
	tmpData = { MesType::STANBY, 0,0 };
	state_->SetActive(ActiveState::Stanby);
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Stanby : ���M���s\n");
	}
}

void NetWork::SendStart(void)
{
	MesData tmpData;
	tmpData = { MesType::GAME_START, 0,0 };
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Start : ���M���s\n");
	}
}

bool NetWork::SendMes(MesData& data)
{
	if (NetWorkSend(state_->GetNetHandle(), &data, sizeof(MesData)) == -1)
	{
		TRACE("�f�[�^�̑��M���s\n");
		return false;
	}
	return true;
}

bool NetWork::ConnectHost(IPDATA hostIP)
{
	if (state_)
	{
		return state_->ConnectHost(hostIP);
	}
	TRACE("���[�h���ݒ肳��Ă��܂���\n");
	return false;
}

NetWork::NetWork()
{
	recvStanby_ = false;
	tmp_ = 0;
	tmxSize_ = 0;
}

NetWork::~NetWork()
{
}
