#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include <DxLib.h>

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::sInstance_(new NetWork);

IPDATA NetWork::GetIP()
{
	IPDATA ip;
	GetMyIPAddress(&ip);
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
		TRACE("例外：オフラインに設定します。");
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
					TRACE("合図を確認、ゲームを開始します\n");
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
				recvStanby_ = true;
				state_->SetActive(ActiveState::Play);
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

bool NetWork::GetReceiveData(void)
{
	if (GetNetWorkDataLength(state_->GetNetHandle()) > sizeof(MesData))
	{
		MesData data;
		NetWorkRecv(state_->GetNetHandle(), &data, sizeof(MesData));

		return true;
	}
	return false;
}

void NetWork::SendStanby(void)
{
	// 初期化情報の送信
	MesData tmpData;
	tmpData = { MesType::STANBY, 0,0 };
	state_->SetActive(ActiveState::Stanby);
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Stanby : 送信失敗");
	}
}

void NetWork::SendStart(void)
{
	MesData tmpData;
	tmpData = { MesType::GAME_START, 0,0 };
	if (NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData)) == -1)
	{
		TRACE("Start : 送信失敗");
	}
}

bool NetWork::SendMes(MesData& data)
{
	MesData tmpData;
	tmpData = {MesType::POS, data.data[0],data.data[1] };
	NetWorkSend(state_->GetNetHandle(), &tmpData, sizeof(MesData));
	return true;
}

bool NetWork::ConnectHost(IPDATA hostIP)
{
	return state_->ConnectHost(hostIP);
}

NetWork::NetWork()
{
}

NetWork::~NetWork()
{
}
