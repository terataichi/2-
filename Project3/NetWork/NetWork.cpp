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
	MesH revHeader{};
	UnionVec tmpPacket{};
	int writePos = 0;
	listIntP handlelist;
	int handle = -1;
	while (ProcessMessage() == 0)
	{
		if (!state_->Update())
		{
			// �g�������̂����Z�b�g����
			//state_.reset();
			recvStanby_ = false;
			continue;
		}
		handlelist = state_->GetNetHandle();
		handle = handlelist.front().first;
		if (handle != -1)
		{
			break;
		}
	}

	while (ProcessMessage() == 0 && state_->Update())
	{

		// �f�[�^�̒����`�F�b�N
		if (GetNetWorkDataLength(handle) >= sizeof(MesH))
		{
			// ����ȏ�f�[�^���Ȃ��Ƃ�
			// ���w�b�_�[������M����O���
			if (!revHeader.next)
			{
				tmpPacket.resize(0);
				writePos = 0;
			}

			// �w�b�_�[���̎�M
			NetWorkRecv(handle, &revHeader, sizeof(MesH));

			// �f�[�^������ꍇ
			if (revHeader.length)
			{
				tmpPacket.resize(tmpPacket.size() + revHeader.length);

				// �f�[�^�̎󂯎��
				NetWorkRecv(handle, &tmpPacket[writePos], sizeof(int) * revHeader.length);
				writePos = tmpPacket.size();
			}

			// �܂��f�[�^���c���Ă�ꍇ
			if (revHeader.next)
			{
				continue;
			}

			unsigned int type = static_cast<unsigned int>(revHeader.type) - static_cast<unsigned int>(MesType::NON);
			revUpdate_[type](revHeader, tmpPacket);
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
	DxLib::CloseNetWork(state_->GetNetHandle().front().first);
	return true;
}

ActiveState NetWork::GetActive(void)
{
	return state_->GetActive();
}

void NetWork::SendStanby(void)
{
	// ���������̑��M
	state_->SetActive(ActiveState::Stanby);
	SendMes(MesType::STANBY);
}

void NetWork::SendStart(void)
{
	// ���������̑��M
	SendMes(MesType::GAME_START);
}

bool NetWork::AddRevList(std::mutex& mtx, RevDataListP& data)
{
	// �ǉ�
	revDataList_.emplace_back(std::pair<std::mutex&, RevDataListP&>{ mtx, data });
	return true;
}

void NetWork::RunUpDate(void)
{
	update = std::thread(&NetWork::Update, this);
	update.detach();
}

void NetWork::SetHeader(UnionHeader& header, UnionVec& packet)
{
	packet.insert(packet.begin(), { header.iData[1] });
	packet.insert(packet.begin(), { header.iData[0] });
}

bool NetWork::SendMes(MesType type,UnionVec packet)
{
	if (state_->GetNetHandle().front().first == -1)
	{
		return false;
	}

	// �󂯎����MesType�Ńw�b�_�[�𐶐����āAMesPacket�̐擪�ɑ}������B
	UnionHeader header{ type,0,0 };
	SetHeader(header, packet);

	// ���߂����M�f�[�^������w�b�_�[�T�C�Y�������������w�b�_�[��length�ɓ����B
	do
	{
		// ���M�f�[�^�������߂�
		int sendCount = (packet.size() < 500 / sizeof(UnionData)) ? packet.size() : 500 / sizeof(UnionData);

		// ���߂����M�f�[�^������w�b�_�[�T�C�Y�������������w�b�_�[��length�ɓ����
		header.mesH.length = sendCount - HEADER_COUNT;

		// �������Ȃ��Ƃ����Ȃ����ǂ���
		if (packet.size() == sendCount)
		{
			//TRACE("�������܂���\n");
			header.mesH.next = 0;
		}
		else
		{
			//TRACE("�������܂�\n");
			//TRACE("%d���\n", header.mesH.sendID);
			header.mesH.next = 1;
		}

		// �w�b�_�[���̍X�V
		packet[0].iData = header.iData[0];
		packet[1].iData = header.iData[1];


		// �f�[�^�̑��M
		NetWorkSend(state_->GetNetHandle().front().first, packet.data(), sendCount * sizeof(UnionData));

		// �������v�f�̂ݍ폜
		packet.erase(packet.begin() + HEADER_COUNT, packet.begin() + sendCount);

		// ����x�ɃJ�E���g++
		header.mesH.sendID++;

		// ��x�͑����Ăق�������do
		// �����f�[�^������I���܂Ń��[�v
	} while (packet.size() > HEADER_COUNT);

	return true;
}

bool NetWork::SendMes(MesType type)
{
	UnionVec vec{};
	return SendMes(type, vec);
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
	Init();
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
	auto gameStart = [&](MesH& data,UnionVec& packet)
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
	auto stanby = [&](MesH& data, UnionVec& packet)
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

	auto tmx_Size = [&](MesH& data, UnionVec& packet)
	{
		// �c�������������C���[��
		tmxSize_ = packet[0].cData[0] * packet[0].cData[1] * packet[0].cData[2];
		revBox_.resize(tmxSize_);

		TRACE("TMX�f�[�^�T�C�Y�́F%d\n", tmxSize_);
		start_ = std::chrono::system_clock::now();
		return true;
	};

	auto  tmx_Data = [&](MesH& data, UnionVec& packet)
	{
		revBox_ = packet;
		return true;
	};

	auto addList = [&](MesH& data, UnionVec& packet)
	{
		if (packet.size())
		{
			std::lock_guard<std::mutex> lock(revDataList_[packet[0].iData / 5].first);
			revDataList_[packet[0].iData / 5].second.emplace_back(data, packet);
		}
		return true;

	};

	auto non = [&](MesH& data, UnionVec& packet)
	{
		TRACE("Nondayoooooooooooo\n");
		return true;
	};

	auto countDown = [&](MesH& data, UnionVec& packet) 
	{
		return true;
	};

	auto id = [&](MesH& data, UnionVec& packet)
	{
		playerID_ = packet[0].iData;
		playerMax_ = packet[0].iData;
		state_->SetPlayerID(playerID_);
		return true;
	};

	auto startTime = [&](MesH& data, UnionVec& packet)
	{
		return true;
	};

	revUpdate_[0] = non;
	revUpdate_[1] = countDown;
	revUpdate_[2] = id;
	revUpdate_[3] = stanby;
	revUpdate_[4] = startTime;
	revUpdate_[5] = gameStart;
	revUpdate_[6] = tmx_Size;
	revUpdate_[7] = tmx_Data;
	revUpdate_[8] = addList;
	revUpdate_[9] = addList;
	revUpdate_[10] = addList;
	revUpdate_[11] = addList;
}

void NetWork::Init(void)
{
	recvStanby_ = false;
	tmp_ = 0;
	tmxSize_ = 0;
	ip_ = ArrayIP{};
	sendLength_ = 0;


	// �o�C�g���̓ǂݍ���
	std::ifstream ifs("init/setting.txt");
	if (ifs.fail())
	{
		TRACE("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n");
		return;
	}
	// �t�@�C���̓��e���i�[
	std::string data;
	getline(ifs, data);
	if (data.find("byte"))
	{
		std::istringstream is{ data.substr(data.find_first_of("\"")+ 1, data.find_last_of("\"") - 1)};
		std::string length;
		getline(is, length);
		sendLength_ = atoi(length.c_str());
	}

}
