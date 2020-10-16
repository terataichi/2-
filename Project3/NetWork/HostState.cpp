#include <DxLib.h>
#include "HostState.h"

HostState::HostState()
{
	//active_ = !PreparationListenNetWork(portNum_);
	bool flg;
	flg = PreparationListenNetWork(portNum_) == 0 ? true : false;

	active_ = ActiveState::Non;

	if (flg)
	{
		// Ú‘±‘Ò‚¿‚É“ü‚é
		active_ = ActiveState::Wait;
	}
}

HostState::~HostState()
{
}

//ActiveState HostState::CheckPreparation(void)
//{
//	bool flg;
//	flg = PreparationListenNetWork(portNum_) == 0 ? true : false;
//
//	// Ú‘±‘Ò‚¿ó‘Ô‚É‚·‚é
//	if (flg) return ActiveState::Wait;
//
//	return ActiveState::Non;
//}

bool HostState::CheckConnect(void)
{
	int handle = GetNewAcceptNetWork();

	if(handle != -1)
	{
		// Ú‘±‚³‚ê‚Ä‚é‚Ì‚Å‚±‚êˆÈãÚ‘±‚³‚ê‚È‚¢‚æ‚¤‚É~‚ß‚é
		StopListenNetWork();
		// ‰Šú‰»ó‘Ô‚É“ü‚é
		netHandle_ = handle;
		active_ = ActiveState::Init;

		TRACE("Ú‘±‚ªŠm”F‚³‚ê‚Ü‚µ‚½");
		return true;
	}
	return false;
}
