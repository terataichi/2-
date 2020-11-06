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

		if (!state_->Update())
		{
			// �g�������̂����Z�b�g����
			recvStanby_ = false;
			continue;
		}
		auto handle = state_->GetNetHandle();
		if (handle == -1)
		{
			continue;
		}

		// �w�b�_�[���̎�M
		if (GetNetWorkDataLength(state_->GetNetHandle()) >= sizeof(MesH))
		{
			MesH data;
			NetWorkRecv(state_->GetNetHandle(), &data, sizeof(MesH));

			// �f�[�^�̑傫���������Ă���

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
			TRACE("type : %d\n", data.type);
		}


		//if (guestRevMap_.find(data.type) != guestRevMap_.end())
		//{
		//	guestRevMap_[data.type](data, SendVec);
		//}
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
	UnionVec vecData;
	UnionHeader mData{ MesType::STANBY,0, 0,0 };

	SetHeader(mData, vecData);
	state_->SetActive(ActiveState::Stanby);

	if (NetWorkSend(state_->GetNetHandle(), &vecData[0], vecData.size() * sizeof(UnionData)) == -1)
	{
		TRACE("Stanby : ���M���s\n");
	}
}

void NetWork::SendStart(void)
{
	// ���������̑��M
	UnionVec vecData;

	UnionHeader mData{ MesType::GAME_START,0, 0,0 };

	SetHeader(mData, vecData);
	if (NetWorkSend(state_->GetNetHandle(), &vecData[0], vecData.size() * sizeof(UnionData)) == -1)
	{
		TRACE("Start : ���M���s\n");
	}
}

void NetWork::RunUpDate(void)
{
	update=std::thread(&NetWork::Update, this);
	update.detach();
}

void NetWork::SetHeader(UnionHeader header, UnionVec& vec)
{
	vec.insert(vec.begin(), { header.iData[1] });
	vec.insert(vec.begin(), { header.iData[0] });
}

bool NetWork::SendMes(UnionVec& data)
{
	if (NetWorkSend(state_->GetNetHandle(), &data[0], data.size() * sizeof(UnionData)) == -1)
	{
		TRACE("�w�b�_�[���̑��M���s\n");
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
	InitFunc();
}

NetWork::~NetWork()
{
	if (update.joinable())
	{
		update.join();
	}
}

void NetWork::InitFunc(void)
{
	// ---- �z�X�g ---
	auto hostStanby = [&](MesH& data)
	{
		// �Q�[���X�^�[�g����M��
		if (state_->GetActive() == ActiveState::Stanby)
		{
			// �Q�[���X�^�[�g����M��
			if (static_cast<MesType>(data.type) == MesType::GAME_START)
			{
				TRACE("�Q�X�g����ʒm���m�F�A�Q�[�����J�n���܂�\n");
				state_->SetActive(ActiveState::Play);
				return true;
			}
		}
		return false;
	};


	// ---- �Q�X�g ---
	auto guestStanby = [&](MesH& data)
	{
		std::ofstream ofs("TileMap/SendData.tmx", std::ios::out);			// �������ݗp

		std::ifstream ifs("TileMap/Stage01_FileData.dat", std::ios::in | std::ios::binary);					// �w�b�_�[�ǂݍ��ݗp
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
				ofs << str;

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
		for (auto box : revBox_)
		{
			for (int i = 0; i < 8; i++)
			{
				// �����̑}��
				ofs << (box.cData[(i / 2) % 4] >> (4 * (i % 2)) & 0x0f);

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
					end_ = std::chrono::system_clock::now();
					// �b�ɕϊ�
					auto time = std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count();
					//TRACE("�󂯎�肩�瑗��܂ł����������Ԃ� : %d\n", time);
					std::cout << time << std::endl;
					break;
				}
			}
		}
		ifs.close();
		return true;
	};

	auto tmx_Size = [&](MesH& data)
	{
		if (data.length > 0)
		{
			if (GetNetWorkDataLength(state_->GetNetHandle()) <= static_cast<int>(data.length))
			{
				TRACE("�f�[�^��Length��菬�����ł�\n");
			}
			UnionVec SendVec;
			SendVec.resize(data.length);
			NetWorkRecv(state_->GetNetHandle(), &SendVec[0], sizeof(int) * data.length);

			tmxSize_ = SendVec[0].iData;
			revBox_.resize(tmxSize_);
		}

		TRACE("TMX�f�[�^�T�C�Y�́F%d\n", tmxSize_);
		start_ = std::chrono::system_clock::now();
		return true;
	};

	auto  tmx_Data = [&](MesH& data)
	{
		if (data.length > 0)
		{
			if (GetNetWorkDataLength(state_->GetNetHandle()) <= static_cast<int>(data.length))
			{
				TRACE("�f�[�^��Length��菬�����ł�\n");
			}
			NetWorkRecv(state_->GetNetHandle(), &revBox_[0], data.length * sizeof(UnionData));
		}
		return true;
	};

	hostRevMap_.try_emplace(MesType::GAME_START, hostStanby );

	guestRevMap_.try_emplace(MesType::STANBY, guestStanby);
	guestRevMap_.try_emplace(MesType::TMX_DATA, tmx_Data);
	guestRevMap_.try_emplace(MesType::TMX_SIZE, tmx_Size);

}
