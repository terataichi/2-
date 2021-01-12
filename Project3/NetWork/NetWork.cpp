#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include <DxLib.h>
#include <chrono>
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
	while (ProcessMessage() == 0)
	{
		auto lostHandle = GetLostNetWork();

		for (auto list = handlelist_.begin(); list != handlelist_.end(); list++)
		{
			if (lostHandle == list->handle_)
			{
				TRACE("�ؒf���ꂽ�n���h���폜\n");
				handlelist_.erase(list);
				playerMax_--;
				TRACE("-max :%d,", playerMax_);
				break;
			}
		}

		if (!state_->Update() || !handlelist_.size())
		{
			// �g�������̂����Z�b�g����
			//state_.reset();
			recvStanby_ = false;
		}
		else
		{
			break;
		}
	}

	while (ProcessMessage() == 0 && handlelist_.size() && !endFlg_)
	{
		auto lostHandle = GetLostNetWork();
		int lengthCnt = 0;
		// �f�[�^�̒����`�F�b�N
		for (auto list = handlelist_.begin(); list != handlelist_.end(); list++)
		{
			if (lostHandle == list->handle_)
			{
				TRACE("�ؒf���ꂽ�n���h���폜\n");

				if (revDataList_.size())
				{
					std::lock_guard<std::mutex> lock(revDataList_[list->id_ / 5].first);
					UnionData data;
					data.iData = list->id_;
					UnionVec packet{ data };
					SendMesAll(MesType::LOST, packet, 0);
					revDataList_[list->id_ / 5].second.emplace_back(MesH{ MesType::LOST,0,0,0 }, packet);
				}
				handlelist_.erase(list);
				playerMax_--;
				break;
			}

			if (GetNetWorkDataLength(list->handle_) >= sizeof(MesH))
			{
				// ����ȏ�f�[�^���Ȃ��Ƃ�
				// ���w�b�_�[������M����O���
				if (!revHeader.next)
				{
					tmpPacket.resize(0);
					writePos = 0;
				}

				// �w�b�_�[���̎�M
				NetWorkRecv(list->handle_, &revHeader, sizeof(MesH));

				// �f�[�^������ꍇ
				if (revHeader.length)
				{
					if (tmpPacket.max_size() > static_cast<size_t>(tmpPacket.size() + revHeader.length))
					{
						tmpPacket.resize(tmpPacket.size() + revHeader.length);

						// �f�[�^�̎󂯎��
						NetWorkRecv(list->handle_, &tmpPacket[writePos], sizeof(int) * revHeader.length);
						writePos = tmpPacket.size();
					}
					else
					{
						TRACE("�f�[�^�T�C�Y���傫�����܂�\n");
					}
				}
				// �܂��f�[�^���c���Ă�ꍇ
				if (revHeader.next)
				{
					lengthCnt++;
					// TRACE("%d\n", lengthCnt);
					continue;
				}
				lengthCnt = 0;

				// 

				if (revHeader.length < 500 && revHeader.length >= 0)
				{

					if (static_cast<int>(revHeader.type) >= static_cast<int>(MesType::NON) &&
						static_cast<int>(revHeader.type) <= static_cast<int>(MesType::MAX))
					{
						if (revHeader.type == MesType::POS)
						{
							if (revHeader.length == 6)
							{
								SendMesAll(revHeader.type, tmpPacket, list->handle_);

								unsigned int type = static_cast<int>(revHeader.type) - static_cast<int>(MesType::NON);
								revUpdate_[type](revHeader, tmpPacket);
							}
							else
							{
								TRACE("pos\n");
							}
						}
						else
						{
							SendMesAll(revHeader.type, tmpPacket, list->handle_);

							unsigned int type = static_cast<int>(revHeader.type) - static_cast<int>(MesType::NON);
							revUpdate_[type](revHeader, tmpPacket);
						}

					}
					else
					{
						TRACE("���b�Z�[�W�^�C�v���� ID : %d\n", list->id_);
					}
				}
			}
		}
	}

	// �I�t���C���ȊO��������ؒf�����
	if (state_->GetMode() != NetWorkMode::OFFLINE)
	{
		TRACE("�I�����C�����I�����܂�\n");
		if (handlelist_.size())
		{
			CloseNetWork();
			Init();
			InitFunc();
		}
	}

	return true;
}

bool NetWork::CloseNetWork(void)
{
	for (auto list = handlelist_.begin(); list != handlelist_.end(); list++)
	{
		DxLib::CloseNetWork(list->handle_);
	}	return true;
}

ActiveState NetWork::GetActive(void)
{
	return state_->GetActive();
}

void NetWork::SendStanby(void)
{
	// ���������̑��M
	state_->SetActive(ActiveState::Stanby);
	SendMesAll(MesType::STANBY_HOST);
}

void NetWork::SendStart(void)
{
	// ���������̑��M
	SendMes(MesType::STANBY_GUEST);
}

bool NetWork::CheckNetWork()
{
	return state_->CheckNetWork();
}

bool NetWork::EndNetWork(void)
{
	endFlg_ = true;
	return endFlg_;
}

chronoTime NetWork::GetStartTime(void)
{
	return countDownTime_;
}

void NetWork::SetStartTime(chronoTime time)
{
	countDownTime_ = time;
}

bool NetWork::GetCountDownFlg(void)
{
	return countDownFlg_;
}

void NetWork::SetCountDownFlg(bool flg)
{
	countDownFlg_ = flg;
}

void NetWork::SetStartCntFlg(bool flg)
{
	startCntFlg_ = flg;
}

bool NetWork::GetStartCntFlg(void)
{
	return startCntFlg_;
}

void NetWork::AddPlayerMax()
{
	playerMax_++;
}

const int NetWork::GetPlayerMax(void) const
{
	return playerMax_;
}

const int NetWork::GetPlayerID(void) const
{
	return playerID_;
}

const UnionVec& NetWork::GetResultData(void) const
{
	return resultData_;
}

void NetWork::AddHandleList(PlayerHandle intp)
{
	handlelist_.emplace_front(intp);
}

listIntP& NetWork::GetHandleList(void)
{
	return handlelist_;
}

bool NetWork::AddRevList(std::mutex& mtx, RevDataListP& data)
{
	// �ǉ�
	revDataList_.emplace_back(std::pair<std::mutex&, RevDataListP&>{ mtx, data });
	return true;
}

void NetWork::RunUpDate(void)
{
	update_ = std::thread(&NetWork::Update, this);
	update_.detach();
}

void NetWork::SetHeader(UnionHeader& header, UnionVec& packet)
{
	packet.insert(packet.begin(), { header.iData[1] });
	packet.insert(packet.begin(), { header.iData[0] });
}

bool NetWork::SendMes(MesType type,UnionVec packet)
{
	if (!handlelist_.size())
	{
		return false;
	}

	SendMes(type, packet, handlelist_.front().handle_);

	return true;
}

bool NetWork::SendMes(MesType type, UnionVec packet, int handle)
{
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
		NetWorkSend(handle, packet.data(), sendCount * sizeof(UnionData));

		// �������v�f�̂ݍ폜
		packet.erase(packet.begin() + HEADER_COUNT, packet.begin() + sendCount);

		// ����x�ɃJ�E���g++
		header.mesH.sendID++;

		// ��x�͑����Ăق�������do
		// �����f�[�^������I���܂Ń��[�v
	} while (packet.size() > HEADER_COUNT);
	return false;
}

bool NetWork::SendMesAll(MesType type, UnionVec packet, int handle)
{
	//if (state_->GetNetHandle().size())
	//{
	//	if (state_->GetNetHandle().front().first == -1)
	//	{
	//		return false;
	//	}
	//}

	for (auto list = handlelist_.begin(); list != handlelist_.end(); list++)
	{
		if (list->handle_ == -1)
		{
			continue;
		}

		if (list->handle_ != handle)
		{
			// �󂯎����MesType�Ńw�b�_�[�𐶐����āAMesPacket�̐擪�ɑ}������B
			UnionHeader header{ type,0,0 };
			UnionVec tmpPacket = packet;
			SetHeader(header, tmpPacket);
			// ���߂����M�f�[�^������w�b�_�[�T�C�Y�������������w�b�_�[��length�ɓ����B
			do
			{
				// ���M�f�[�^�������߂�
				int sendCount = (tmpPacket.size() < 500 / sizeof(UnionData)) ? tmpPacket.size() : 500 / sizeof(UnionData);

				// ���߂����M�f�[�^������w�b�_�[�T�C�Y�������������w�b�_�[��length�ɓ����
				header.mesH.length = sendCount - HEADER_COUNT;

				// �������Ȃ��Ƃ����Ȃ����ǂ���
				if (tmpPacket.size() == sendCount)
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
				tmpPacket[0].iData = header.iData[0];
				tmpPacket[1].iData = header.iData[1];


				// �f�[�^�̑��M
				if (handlelist_.size())
				{
					NetWorkSend(list->handle_, tmpPacket.data(), sendCount * sizeof(UnionData));
				}

				// �������v�f�̂ݍ폜
				tmpPacket.erase(tmpPacket.begin() + HEADER_COUNT, tmpPacket.begin() + sendCount);

				// ����x�ɃJ�E���g++
				header.mesH.sendID++;

				// ��x�͑����Ăق�������do
				// �����f�[�^������I���܂Ń��[�v
			} while (tmpPacket.size() > HEADER_COUNT);
		}
	}
	return false;
}

bool NetWork::SendMes(MesType type)
{
	UnionVec vec{};
	return SendMes(type, vec);
}

bool NetWork::SendMesAll(MesType type)
{
	UnionVec vec{};
	return SendMesAll(type, vec, 0);
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
	if (update_.joinable())
	{
		update_.join();
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
			if (static_cast<MesType>(data.type) == MesType::STANBY_GUEST)
			{
				TRACE("�Q�X�g����ʒm���m�F\n"); 
				stanbyCnt_++;
				// �����̕��������Ă���
				if (playerMax_ - 1  <= stanbyCnt_)
				{
					// �J�E���g�_�E���̑��M
					UnionData data[2];
					TimeData time{ lpSceneMng.GetTime() };

					data[0].iData = time.iData[0];
					data[1].iData = time.iData[1];

					lpNetWork.SetStartTime(time.time);
					lpNetWork.SetStartCntFlg(true);
					lpNetWork.SendMesAll(MesType::COUNT_DOWN_GAME, UnionVec{ data[0],data[1] }, 0);

					state_->SetActive(ActiveState::Play);
					playerID_ = 0;
				}
				return true;
			}
		}
		return false;
	};

	// ---- �Q�X�g ---
	auto stanby = [&](MesH& data, UnionVec& packet)
	{
		if (loadStage_ > 0)
		{
			TRACE("�ð�ޏ��%d�񑗂��Ă��Ă܂�\n",loadStage_);
			return false;
		}
		loadStage_++;
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
			if (data.type == MesType::LOST)
			{
				TRACE("�N�����ؒf\n");
			}

			if (mesTypeSize_[data.type] != packet.size())
			{
				TRACE("�{�}�[������\n");
				return false;
			}

			int id = packet[0].iData / 5;

			if (id < static_cast<int>(revDataList_.size()))
			{
				if (id <= playerMax_)
				{
					// if (packet[0].iData > 0)
					{
						std::lock_guard<std::mutex> lock(revDataList_[id].first);
						auto a = (revDataList_[id].second.size() + packet.size());
						auto b = revDataList_[id].second.max_size();

						if (packet.size() > 5000)
						{
							int unko = 0;
						}

						if (b > a && a > 0)
						{
							revDataList_[id].second.emplace_back(data, packet);
						}
						else
						{
							int unchi = 0;
						}
					}
				}
			}
		}
		return true;
	};

	auto non = [&](MesH& data, UnionVec& packet)
	{
		TRACE("Non\n");
		return true;
	};

	auto countDown = [&](MesH& data, UnionVec& packet) 
	{
		TimeData timeData{};
		timeData.iData[0] = packet[0].iData;
		timeData.iData[1] = packet[1].iData;

		countDownFlg_ = true;
		countDownTime_ = timeData.time;
		return true;
	};

	auto id = [&](MesH& data, UnionVec& packet)
	{
		// ��񂾂��ݒ�
		if (packet[0].iData % 5 == 0)
		{
			if (playerID_ == -1 && packet[0].iData / 5 <= packet[1].iData)
			{
				playerID_ = packet[0].iData;
				playerMax_ = packet[1].iData;
				state_->SetPlayerID(playerID_);
			}
		}
		TRACE("id : %d", playerID_);
		TRACE("max : %d\n", playerMax_);

		return true;
	};

	auto startTime = [&](MesH& data, UnionVec& packet)
	{
		TimeData timeData{};
		timeData.iData[0] = packet[0].iData;
		timeData.iData[1] = packet[1].iData;

		startCntFlg_ = true;
		countDownTime_ = timeData.time;

		chronoTime now = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - countDownTime_).count();
		auto tmp = (COUNT_START_MAX - time) / 1000;
		if (tmp < 0)
		{
			countDownTime_ = lpSceneMng.GetTime();
		}
		return true;
	};

	auto result = [&](MesH& data, UnionVec& packet)
	{
		if (packet.size() == 5)
		{
			resultData_ = packet;
		}
		return true;
	};

	revUpdate_[0] = non;
	revUpdate_[1] = countDown;
	revUpdate_[2] = id;
	revUpdate_[3] = stanby;
	revUpdate_[4] = gameStart;
	revUpdate_[5] = startTime;
	revUpdate_[6] = tmx_Size;
	revUpdate_[7] = tmx_Data;
	revUpdate_[8] = addList;
	revUpdate_[9] = addList;
	revUpdate_[10] = addList;
	revUpdate_[11] = result;
	revUpdate_[12] = addList;
}

void NetWork::Init(void)
{
	recvStanby_ = false;
	tmp_ = 0;
	tmxSize_ = 0;
	ip_ = ArrayIP{};
	sendLength_ = 0;
	state_.reset();
	countDownFlg_ = false;
	startCntFlg_ = false;
	stanbyCnt_ = 0;
	playerID_ = -1;
	playerMax_ = 0;
	revDataList_.clear();
	resultData_.clear();
	revBox_.clear();
	endFlg_ = false;
	handlelist_.clear();
	loadStage_ = 0;
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


	mesTypeSize_.try_emplace(MesType::NON, 0);
	mesTypeSize_.try_emplace(MesType::COUNT_DOWN_GAME, 2);
	mesTypeSize_.try_emplace(MesType::COUNT_DOWN_ROOM, 2);
	mesTypeSize_.try_emplace(MesType::DETH, 1);
	mesTypeSize_.try_emplace(MesType::ID, 2);
	mesTypeSize_.try_emplace(MesType::LOST, 1);
	mesTypeSize_.try_emplace(MesType::MAX, 0);
	mesTypeSize_.try_emplace(MesType::POS, 4);
	mesTypeSize_.try_emplace(MesType::SET_BOMB, 7);
	mesTypeSize_.try_emplace(MesType::STANBY_GUEST, 0);
	mesTypeSize_.try_emplace(MesType::STANBY_HOST, 0);
	mesTypeSize_.try_emplace(MesType::TMX_SIZE, 1);
	mesTypeSize_.try_emplace(MesType::TMX_DATA, 179);

}
