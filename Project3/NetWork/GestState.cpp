#include "GestState.h"

GuestState::GuestState()
{
	active_ = false;
}

GuestState::~GuestState()
{
}

bool GuestState::ConnectHost(IPDATA hostIP)
{
	// ０以上：確立した接続を示すネットワークハンドル(int型の識別値)
	netHandle_ = ConnectNetWork(hostIP, portNum_);
	return  active_ = (0 <= netHandle_);
}

bool GuestState::SetSendData(Vector2 pos)
{
	ReaceiveData tmpPos;
	tmpPos = { pos.x,pos.y };
	NetWorkSend(netHandle_, &tmpPos, sizeof(ReaceiveData));
	return true;
}
