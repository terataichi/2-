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
	while (ProcessMessage() == 0)
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
						continue;
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
					std::ofstream fs("TileMap/SendData.tmx", std::ios::out);

					std::ifstream ifs("TileMap/Stage01_FileData.dat");
					if (ifs.fail())
					{
						TRACE("ファイルの読み込みに失敗しました。\n");
						continue;
					}

					if (!fs)
					{
						TRACE("書き込み用のファイルが開けません\n");
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
					for (auto box : RevBox)
					{
						for (int i = 0; i < 16; i++)
						{
							// 文字の挿入
							fs << (box.cData[(i / 2) % 8] >> (4 * (i % 2)) & 0x0f);

							count++;

							// 文字を入れるときに最後の行以外にコロンを入れる
							if (count % 21 != 0)
							{
								fs << ',';
								continue;
							}

							// 行終わりチェック
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

								// ファイル読み込み完了
								if (ifs.eof())
								{
									TRACE("初期化情報の確認、ゲームを開始の合図をします\n");
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
				if (static_cast<MesType>(data.type) == MesType::TMX_SIZE)
				{
					tmxSize_ = data.data[0];
					RevBox.resize(tmxSize_);
					TRACE("TMXデータサイズは：%d\n", data.data[0]);
					continue;
				}
				if (static_cast<MesType>(data.type) == MesType::TMX_DATA)
				{
					RevBox[data.id].iData[0] = data.data[0];
					RevBox[data.id].iData[1] = data.data[1];

					tmp_++;

					continue;
				}
			}
		}

		if (!state_->Update())
		{
			// 使ったものをリセットする
			recvStanby_ = false;
		}
	}

	// オフライン以外だったら切断するよ
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

void NetWork::RunUpDate(void)
{
	std::thread update(&NetWork::Update, this);
	update.detach();
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
