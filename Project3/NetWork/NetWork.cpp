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
				if (data.type == MesType::GAME_START)
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

			if (data.type == MesType::STANBY)
			{

				std::fstream fs("tmxData.tmx", std::ios::out);

				if (!fs)
				{
					TRACE("書き込み用のファイルが開けません\n");
					return false;
				}

				// データの受け取り
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

				TRACE("初期化情報の確認、ゲームを開始の合図をします\n");
				recvStanby_ = true;
				state_->SetActive(ActiveState::Play);
				return true;
			}
			if (data.type == MesType::TMX_SIZE)
			{
				tmxSize_ = data.data[0];
				RevBox.resize(tmxSize_);
				TRACE("TMXデータサイズは：%d\n",data.data[0]);
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
	tmpData = { MesType::STANBY, 0,0 };
	state_->SetActive(ActiveState::Stanby);
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Stanby : 送信失敗\n");
	}
}

void NetWork::SendStart(void)
{
	MesData tmpData;
	tmpData = { MesType::GAME_START, 0,0 };
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
}

NetWork::~NetWork()
{
}
