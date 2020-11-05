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
			UnionVec SendVec;
			SendVec.resize(data.length);
			if (GetNetWorkDataLength(state_->GetNetHandle()) <= data.length)
			{
				TRACE("�f�[�^��Length��菬�����ł�\n");
			}
			NetWorkRecv(state_->GetNetHandle(), &SendVec[0], data.length);


			if (state_->GetMode() == NetWorkMode::HOST)
			{

				if (state_->GetActive() == ActiveState::Stanby)
				{
					// �Q�[���X�^�[�g����M��
					if (static_cast<MesType>(data.type) == MesType::GAME_START)
					{
						TRACE("�Q�X�g����ʒm���m�F�A�Q�[�����J�n���܂�\n");
						state_->SetActive(ActiveState::Play);
						continue;
					}
				}
			}
			else if (state_->GetMode() == NetWorkMode::GUEST)
			{
				if (data.type == MesType::STANBY)
				{
					std::ofstream fs("TileMap/SendData.tmx", std::ios::out);

					std::ifstream ifs("TileMap/Stage01_FileData.dat");
					if (ifs.fail())
					{
						TRACE("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n");
						continue;
					}

					if (!fs)
					{
						TRACE("�������ݗp�̃t�@�C�����J���܂���\n");
						continue;
					}

					std::string str;
					do
					{
						getline(ifs, str);
						fs << str << std::endl;

						if (ifs.eof())
						{
							break;
						}
					} while (str.find("data encoding") == std::string::npos);

					int count = 0;
					for (auto box : revBox_)
					{
						for (int i = 0; i < 16; i++)
						{
							// �����̑}��
							fs << (box.cData[(i / 2) % 8] >> (4 * (i % 2)) & 0x0f);

							count++;

							// ����������Ƃ��ɍŌ�̍s�ȊO�ɃR����������
							if (count % 21 != 0)
							{
								fs << ',';
								continue;
							}

							// �s�I���`�F�b�N
							if (count % (21 * 17) != 0)
							{
								fs << ',';
								fs << std::endl;
								continue;
							}

							fs << std::endl;
							bool flg = false;
							do
							{
								getline(ifs, str);

								// �t�@�C���ǂݍ��݊���
								if (ifs.eof())
								{
									TRACE("���������̊m�F�A�Q�[�����J�n�̍��}�����܂�\n");
									recvStanby_ = true;
									state_->SetActive(ActiveState::Play);
									flg = true;
									break;
								}
								fs << str << std::endl;
							} while (str.find("data encoding") == std::string::npos);

							if (flg)
							{
								break;
							}
						}
					}
				}
				if (data.type == MesType::TMX_SIZE)
				{
					tmxSize_ = SendVec[1].iData;
					revBox_.resize(tmxSize_);
					TRACE("TMX�f�[�^�T�C�Y�́F%d\n", tmxSize_);
					continue;
				}
				if (data.type == MesType::TMX_DATA)
				{
					auto kotaro = 20;
					continue;
				}
			}
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
	vecData.resize(2);
	UnionHeader mData{ MesType::STANBY,0, 0,0 };

	vecData[0].iData = mData.iData[0];
	vecData[1].iData = mData.iData[1];

	//MesH tmpData;
	//tmpData = { MesType::STANBY,0, 0,0 };
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
	vecData.resize(2);

	UnionHeader mData{ MesType::GAME_START,0, 0,0 };
	vecData[0].iData = mData.iData[0];
	vecData[1].iData = mData.iData[1];

	//MesH tmpData;
	//tmpData = {(MesType::GAME_START),0, 0,0 };
	if (NetWorkSend(state_->GetNetHandle(), &vecData[0], vecData.size() * sizeof(UnionData)) == -1)
	{
		TRACE("Start : ���M���s\n");
	}
}

void NetWork::RunUpDate(void)
{
	std::thread update(&NetWork::Update, this);
	update.detach();
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
}

void NetWork::InitFunc(void)
{
	//// ---- �z�X�g ---
	//auto hostStanby = [&](MesH& data,UnionVec& vec)
	//{
	//	// �Q�[���X�^�[�g����M��
	//	if (data.type == MesType::GAME_START)
	//	{
	//		TRACE("�Q�X�g����ʒm���m�F�A�Q�[�����J�n���܂�\n");
	//		state_->SetActive(ActiveState::Play);
	//		return true;
	//	}
	//	return false;
	//};


	//// ---- �Q�X�g ---
	//auto guestStanby = [&](MesH& data, UnionVec& vec)
	//{
	//	std::ofstream ofs("TileMap/SendData.tmx", std::ios::out);			// �������ݗp

	//	std::ifstream ifs("TileMap/Stage01_FileData.dat");					// �w�b�_�[�ǂݍ��ݗp
	//	if (ifs.fail())
	//	{
	//		TRACE("�ǂݍ��ݗp�t�@�C�����J���܂���\n");
	//		return false;
	//	}

	//	if (ofs.fail())
	//	{
	//		TRACE("�������ݗp�̃t�@�C�����J���܂���\n");
	//		return false;
	//	}

	//	// �w�b�_�[�����̏������ݗp�����_��
	//	std::string str;
	//	auto getlineHeader = [&]()
	//	{
	//		do
	//		{
	//			getline(ifs, str);
	//			ofs << str << std::endl;

	//			// �w�b�_�[�̓ǂݍ��݊����`�F�b�N
	//			if (ifs.eof())
	//			{
	//				TRACE("���������̊m�F�A�Q�[�����J�n�̍��}�����܂�\n");
	//				recvStanby_ = true;
	//				state_->SetActive(ActiveState::Play);
	//				return true;
	//			}
	//		} while (str.find("data encoding") == std::string::npos);

	//		return false;
	//	};


	//	// �ŏ��Ɉ��ǂݍ���ł���
	//	getlineHeader();

	//	// �f�[�^����������
	//	int count = 0;
	//	for (auto box : revBox_)
	//	{
	//		for (int i = 0; i < 16; i++)
	//		{
	//			// �����̑}��
	//			ofs << (box.cData[(i / 2) % 8] >> (4 * (i % 2)) & 0x0f);

	//			count++;

	//			// ����������Ƃ��ɍŌ�̍s�ȊO�ɃR����������
	//			if (count % 21 != 0)
	//			{
	//				ofs << ',';
	//				continue;
	//			}

	//			// �s�I���`�F�b�N
	//			if (count % (21 * 17) != 0)
	//			{
	//				ofs << ',';
	//				ofs << std::endl;
	//				continue;
	//			}

	//			ofs << std::endl;

	//			// �ǂݍ��݂��I������̂ŏI��
	//			if (getlineHeader())
	//			{
	//				end = std::chrono::system_clock::now();
	//				// �b�ɕϊ�
	//				auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	//				//TRACE("�󂯎�肩�瑗��܂ł����������Ԃ� : %d\n", time);
	//				std::cout << time << std::endl;
	//				break;
	//			}
	//		}
	//	}

	//	return true;
	//};

	//auto tmx_Size = [&](MesH& data, UnionVec& vec)
	//{
	//	// �f�[�^�̎󂯎��
	//	TmxDataSize size;

	//	tmxSize_ = vec[0].iData[0];
	//	revBox_.resize(tmxSize_);
	//	TRACE("TMX�f�[�^�T�C�Y�́F%d\n", tmxSize_);
	//	start = std::chrono::system_clock::now();
	//	return true;
	//};

	//auto  tmx_Data = [&](MesH& data, UnionVec& vec)
	//{
	//	revBox_ = vec.data[0];
	//	revBox_ = vec.data[1];
	//	return true;
	//};

	//hostRevMap_.try_emplace(MesType::STANBY, hostStanby );

	//guestRevMap_.try_emplace(MesType::STANBY, guestStanby);
	//guestRevMap_.try_emplace(MesType::TMX_DATA, tmx_Data);
	//guestRevMap_.try_emplace(MesType::TMX_SIZE, tmx_Size);

}
