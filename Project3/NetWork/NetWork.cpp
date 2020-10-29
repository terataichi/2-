#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <sstream>

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
		break;
	case NetWorkMode::GUEST:
		state_ = std::make_unique<GuestState>();
		break;
	default:
		TRACE("例外：オフラインに設定します。\n");
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

				// ゲームスタートを受信時
				if (static_cast<MesType>(data.type) == MesType::GAME_START)
				{
					TRACE("ゲストから通知を確認、ゲームを開始します\n");
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

			if (static_cast<MesType>(data.type) == MesType::STANBY)
			{
				std::fstream fs("tmxData.tmx", std::ios::out);

				std::ifstream ifs("TileMap/Stage01.tmx");
				if (ifs.fail())
				{
					TRACE("ファイルの読み込みに失敗しました。\n");
					return false;
				}

				if (!fs)
				{
					TRACE("書き込み用のファイルが開けません\n");
					return false;
				}

				// XML部分の先頭の書き込み
				std::string file;
				int count = 0;
				int taichi = 24;
				unsigned int maskbuf = 4278190080;
				// データの書き込み
				while (getline(ifs, file))
				{

					if (file.find("<") != -1)
					{
						fs << file;
						fs << '\n';
					}
					else
					{
						int test = 0;
						int tmp = 0;
						int tmp2 = 0;

						while (test < (17 * 21))
						{
							if (count < RevBox.size())
							{
								
								//unsigned char* ch = reinterpret_cast<unsigned char*>(&RevBox[count]);
								unsigned int num = RevBox[count];

								
								//mask += maskbuf;
								//auto tmmmmp = INT_MAX;
								for (int i = 0; i < sizeof(RevBox[count]); i++)
								{
									
									/*if (ch[i] != 0 && ch[i] != EOF)
									{*/
										tmp++;
										auto number = (num & maskbuf);
										number = number >> taichi;
										taichi -= 8;
										maskbuf = maskbuf >> 8;
										if (taichi < 0)
										{
											taichi = 24;
											maskbuf = 4278190080;
											if (i < 3)
											{
												num = RevBox[count + 1];
											}
										}

										fs << number;
										fs << ',';
										if (tmp % 21 == 0)
										{
											tmp2++;
											fs << "\n";
										}
										//TRACE("%c", ch[i]);
										std::cout << number;
										test++;
										if (tmp2 >= 17)
										{
											count--;
											break;
										}
										
									/*}
									else
									{
										std::cout << "ﾖｼｯ" << std::endl;
									}*/
								}
								count++;
							}
							else
							{
								test = INT_MAX;
							}
						}
						for (int i = 0; i < 17 - 1; i++)
						{
							getline(ifs, file);
						}
					}
				}

				TRACE("初期化情報の確認、ゲームを開始の合図をします\n");
				recvStanby_ = true;
				state_->SetActive(ActiveState::Play);
				return true;
			}
			if (static_cast<MesType>(data.type) == MesType::TMX_SIZE)
			{
				tmxSize_ = data.data[0];
				RevBox.resize(tmxSize_);
				TRACE("TMXデータサイズは：%d\n",data.data[0]);
				return true;
			}
			if (static_cast<MesType>(data.type) == MesType::TMX_DATA)
			{
				RevBox[data.id] = data.data[0];
				RevBox[data.id + 1] = data.data[1];

				char tmp = data.data[1];
				tmp_++;

				return true;
			}
		}
	}

	if (!state_->Update())
	{
		// 使ったものをリセットする
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
	// 初期化情報の送信
	MesData tmpData;
	tmpData = {(MesType::STANBY),0, 0,0 };
	state_->SetActive(ActiveState::Stanby);
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Stanby : 送信失敗\n");
	}
}

void NetWork::SendStart(void)
{
	MesData tmpData;
	tmpData = {(MesType::GAME_START),0, 0,0 };
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Start : 送信失敗\n");
	}
}

bool NetWork::SendMes(MesData& data)
{
	if (NetWorkSend(state_->GetNetHandle(), &data, sizeof(MesData)) == -1)
	{
		TRACE("データの送信失敗\n");
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
	TRACE("モードが設定されていません\n");
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
