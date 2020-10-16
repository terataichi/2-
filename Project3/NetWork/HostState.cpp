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

	// Ú‘±‘Ò‚¿ó‘Ô‚É‚·‚é
	if (flg) return ActiveState::Wait;

	return ActiveState::Non;
}

bool HostState::CheckConnect(void)
{
	int handle = GetNewAcceptNetWork();

	if(handle != -1)
	{
		// Ú‘±‚³‚ê‚Ä‚é‚Ì‚Å‚±‚êˆÈãÚ‘±‚³‚ê‚È‚¢‚æ‚¤‚ÉŽ~‚ß‚é
		// ‰Šú‰»ó‘Ô‚É“ü‚é
		netHandle_ = handle;
		StopListenNetWork();
		active_ = ActiveState::Init;

		return true;
	}
	//if (GetLostNetWork() != -1)
	//{
	//	// ÄÚ‘±‚ÌŠJŽn
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
