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

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}
