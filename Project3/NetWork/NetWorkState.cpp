#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	active_ = ActiveState::Non;

	netUpdateFunc_[ActiveState::Non] = std::bind(&NetWorkState::CheckConnect, this);
	netUpdateFunc_[ActiveState::Init];
	netUpdateFunc_[ActiveState::Stanby];
	netUpdateFunc_[ActiveState::Play];
	netUpdateFunc_[ActiveState::OFFLINE];
}

NetWorkState::~NetWorkState()
{
}

ActiveState NetWorkState::GetActive(void)
{
	return active_;
}

void NetWorkState::SetActive(ActiveState state)
{
	active_ = state;
}

bool NetWorkState::Update(void)
{

	return false;
}

int NetWorkState::GetNetHandle(void)
{
	return netHandle_;
}

ActiveState NetWorkState::ConnectHost(IPDATA hostIP)
{
	return ActiveState::Wait;
}
