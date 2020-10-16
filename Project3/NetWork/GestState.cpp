#include "GestState.h"

GuestState::GuestState()
{
	active_ = ActiveState::Non;
}

GuestState::~GuestState()
{
}

bool GuestState::ConnectHost(IPDATA hostIP)
{
	// ０以上：確立した接続を示すネットワークハンドル(int型の識別値)
	netHandle_ = ConnectNetWork(hostIP, portNum_);

	// 接続出来たら次に行く
	if (0 <= netHandle_)
	{
		active_ = ActiveState::Init;
		return true;
	}
	return false;
}
