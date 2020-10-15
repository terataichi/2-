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
		// �ڑ�����Ă�̂ł���ȏ�ڑ�����Ȃ��悤�Ɏ~�߂�
		netHandle_ = handle;
		StopListenNetWork();
	}

	if (GetLostNetWork() != -1)
	{
		// �Đڑ��̊J�n
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
