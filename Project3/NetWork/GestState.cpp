#include "GestState.h"
#include "NetWork.h"
#include <DxLib.h>

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
	int netHandle = ConnectNetWork(hostIP, portNum_);

	// 接続出来たら次に行く
	if (0 <= netHandle)
	{
		listIntP& list = lpNetWork.GetHandleList();
		list.emplace_back(PlayerHandle{ netHandle, -1 ,0 });
		active_ = ActiveState::Init;
		return true;
	}
	return false;
}

bool GuestState::CheckConnect(void)
{
	return true;
}
