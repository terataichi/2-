#include "GestState.h"

GuestState::GuestState()
{
	active_ = ActiveState::Non;
}

GuestState::~GuestState()
{
}

ActiveState GuestState::ConnectHost(IPDATA hostIP)
{
	// ０以上：確立した接続を示すネットワークハンドル(int型の識別値)
	netHandle_ = ConnectNetWork(hostIP, portNum_);

	// 接続出来たら次に行く
	if (0 <= netHandle_)
	{
		return ActiveState::Init;
	}
	return  ActiveState::Non;
}

bool GuestState::SetSendData(Vector2 pos)
{
	ReaceiveData tmpPos;
	tmpPos = { pos.x,pos.y };
	NetWorkSend(netHandle_, &tmpPos, sizeof(ReaceiveData));
	return true;
}
