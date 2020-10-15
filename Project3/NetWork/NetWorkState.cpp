#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	active_ = false;
}

NetWorkState::~NetWorkState()
{
}

bool NetWorkState::GetActive(void)
{
	return active_;
}

bool NetWorkState::Update(void)
{
	CheckConnect();

	return false;
}

bool NetWorkState::CheckConnect(void)
{
	return false;
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}

bool NetWorkState::GetReceiveData(Vector2& pos)
{
	return false;
}

bool NetWorkState::SetSendData(Vector2 pos)
{
	return false;
}
