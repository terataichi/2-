#include <DxLib.h>
#include "HostState.h"

HostState::HostState()
{
	//active_ = !PreparationListenNetWork(portNum_);
	active_ = PreparationListenNetWork(portNum_) == 0 ? true : false;

	//int handle = GetNewAcceptNetWork();
	//while (handle ==-1)
	//{
	//	netHandle_ = GetNewAcceptNetWork();
	//}
}

HostState::~HostState()
{
}

bool HostState::CheckConnect(void)
{
	int handle = GetNewAcceptNetWork();

	if(handle != -1)
	{
		// 接続されてるのでこれ以上接続されないように止める
		netHandle_ = handle;
		StopListenNetWork();
	}

	if (GetLostNetWork() != -1)
	{
		// 再接続の開始
		PreparationListenNetWork(portNum_);
		return false;
	}

	return true;
}

bool HostState::GetReceiveData(Vector2& pos)
{
	if (GetNetWorkDataLength(netHandle_) > sizeof(ReaceiveData))
	{
		ReaceiveData data;
		NetWorkRecv(netHandle_, &data, sizeof(ReaceiveData));

		pos = { data.x,data.y };
		return true;
	}
	return false;
}
