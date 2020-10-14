#include <DxLib.h>
#include "HostState.h"

HostState::HostState()
{
	//active_ = !PreparationListenNetWork(portNum_);
	active_ = PreparationListenNetWork(portNum_) == 0 ? true : false;
}

HostState::~HostState()
{
}
