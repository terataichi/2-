#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"


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
		TRACE("��O�F�I�t���C���ɐݒ肵�܂��B");
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

bool NetWork::Update(void)
{
	return state_->Update();
}

bool NetWork::GetActive(void)
{
	return state_->GetActive();
}

bool NetWork::GetReceiveData(Vector2& pos)
{
	return state_->GetReceiveData(pos);
}

bool NetWork::SetSendData(Vector2 pos)
{
	return state_->SetSendData(pos);
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
