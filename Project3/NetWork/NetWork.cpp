#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <istream>

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::sInstance_(new NetWork);

ArrayIP NetWork::GetMyIP()
{
	GetMyIPAddress(&ip_[0],ip_.size());
	return ip_;
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
		RunUpDate();
		break;
	case NetWorkMode::GUEST:
		state_ = std::make_unique<GuestState>();
		RunUpDate();
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
	while (ProcessMessage() == 0)
	{

		if (GetNetWorkDataLength(state_->GetNetHandle()) >= sizeof(MesData))
		{
			MesData data;
			NetWorkRecv(state_->GetNetHandle(), &data, sizeof(MesData));

			if (state_->GetMode() == NetWorkMode::HOST)
			{
				if (hostRevMap_.find(data.type) != hostRevMap_.end())
				{
					hostRevMap_[data.type](data);
				}
			}
			else if (state_->GetMode() == NetWorkMode::GUEST)
			{
				if (guestRevMap_.find(data.type) != guestRevMap_.end())
				{
					guestRevMap_[data.type](data);
				}
			}
		}

		if (!state_->Update())
		{
			// �g�������̂����Z�b�g����
			recvStanby_ = false;
		}
	}

	// �I�t���C���ȊO��������ؒf�����
	if (state_->GetMode() != NetWorkMode::OFFLINE)
	{
		CloseNetWork();
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
	tmpData = {(MesType::STANBY),0, 0,0 };
	state_->SetActive(ActiveState::Stanby);
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Stanby : ���M���s\n");
	}
}

void NetWork::SendStart(void)
{
	MesData tmpData;
	tmpData = {(MesType::GAME_START),0, 0,0 };
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Start : ���M���s\n");
	}
}

void NetWork::RunUpDate(void)
{
	std::thread update(&NetWork::Update, this);
	update.detach();
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
	ip_ = ArrayIP{};
}

NetWork::~NetWork()
{
}

void NetWork::InitFunc(void)
{
	// ---- �z�X�g ---
	auto hostStanby = [&](MesData& data)
	{
		// �Q�[���X�^�[�g����M��
		if (static_cast<MesType>(data.type) == MesType::GAME_START)
		{
			TRACE("�Q�X�g����ʒm���m�F�A�Q�[�����J�n���܂�\n");
			state_->SetActive(ActiveState::Play);
			return true;
		}
		return false;
	};


	// ---- �Q�X�g ---
	auto guestStanby = [&](MesData& data)
	{

		std::ofstream ofs("TileMap/SendData.tmx", std::ios::out);			// �������ݗp

		std::ifstream ifs("TileMap/Stage01_FileData.dat");					// �w�b�_�[�ǂݍ��ݗp
		if (ifs.fail())
		{
			TRACE("�ǂݍ��ݗp�t�@�C�����J���܂���\n");
			return false;
		}

		if (ofs.fail())
		{
			TRACE("�������ݗp�̃t�@�C�����J���܂���\n");
			return false;
		}

		// �w�b�_�[�����̏������ݗp�����_��
		std::string str;
		auto getlineHeader = [&]()
		{
			do
			{
				getline(ifs, str);
				ofs << str << std::endl;

				// �w�b�_�[�̓ǂݍ��݊����`�F�b�N
				if (ifs.eof())
				{
					TRACE("���������̊m�F�A�Q�[�����J�n�̍��}�����܂�\n");
					recvStanby_ = true;
					state_->SetActive(ActiveState::Play);
					return true;
				}
			} while (str.find("data encoding") == std::string::npos);

			return false;
		};


		// �ŏ��Ɉ��ǂݍ���ł���
		getlineHeader();

		// �f�[�^����������
		int count = 0;
		for (auto box : RevBox)
		{
			for (int i = 0; i < 16; i++)
			{
				// �����̑}��
				ofs << (box.cData[(i / 2) % 8] >> (4 * (i % 2)) & 0x0f);

				count++;

				// ����������Ƃ��ɍŌ�̍s�ȊO�ɃR����������
				if (count % 21 != 0)
				{
					ofs << ',';
					continue;
				}

				// �s�I���`�F�b�N
				if (count % (21 * 17) != 0)
				{
					ofs << ',';
					ofs << std::endl;
					continue;
				}

				ofs << std::endl;

				// �ǂݍ��݂��I������̂ŏI��
				if (getlineHeader())
				{
					break;
				}
			}
		}

		return true;
	};

	auto tmx_Size = [&](MesData& data)
	{
		// �f�[�^�̎󂯎��
		tmxSize_ = data.data[0];
		RevBox.resize(tmxSize_);
		TRACE("TMX�f�[�^�T�C�Y�́F%d\n", data.data[0]);
		return true;
	};

	auto  tmx_Data = [&](MesData& data)
	{
		RevBox[data.id].iData[0] = data.data[0];
		RevBox[data.id].iData[1] = data.data[1];
		tmp_++;
		return true;
	};

	hostRevMap_.try_emplace(MesType::STANBY, hostStanby );

	guestRevMap_.try_emplace(MesType::STANBY, guestStanby);
	guestRevMap_.try_emplace(MesType::TMX_DATA, tmx_Data);
	guestRevMap_.try_emplace(MesType::TMX_SIZE, tmx_Size);

}
