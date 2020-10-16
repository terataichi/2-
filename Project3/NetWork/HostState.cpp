#include <DxLib.h>
#include "HostState.h"

HostState::HostState()
{
	//active_ = !PreparationListenNetWork(portNum_);
	active_ = ActiveState::Non;
}

HostState::~HostState()
{
}

ActiveState HostState::CheckPreparation(void)
{
	bool flg;
	flg = PreparationListenNetWork(portNum_) == 0 ? true : false;

	// �ڑ��҂���Ԃɂ���
	if (flg) return ActiveState::Wait;

	return ActiveState::Non;
}

bool HostState::CheckConnect(void)
{
	int handle = GetNewAcceptNetWork();

	if(handle != -1)
	{
		// �ڑ�����Ă�̂ł���ȏ�ڑ�����Ȃ��悤�Ɏ~�߂�
		// ��������Ԃɓ���
		netHandle_ = handle;
		StopListenNetWork();
		active_ = ActiveState::Init;

		return true;
	}
	//if (GetLostNetWork() != -1)
	//{
	//	// �Đڑ��̊J�n
	//	PreparationListenNetWork(portNum_);
	//	return false;
	//}

	return false;
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
