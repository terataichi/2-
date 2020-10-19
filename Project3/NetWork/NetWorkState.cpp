#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	active_ = ActiveState::Non;
}

NetWorkState::~NetWorkState()
{
}

ActiveState NetWorkState::GetActive(void)
{
	return active_;
}

void NetWorkState::SetActive(ActiveState state)
{
	active_ = state;
}

bool NetWorkState::Update(void)
{
	// 接続が切れていないかチェック
	return CheckNetWork();
}

bool NetWorkState::CheckNetWork(void)
{
	if (GetLostNetWork() != -1)
	{
		// 再接続の開始
		TRACE("接続が切れました、再接続します\n");
		return false;
	}
	return true;
}

int NetWorkState::GetNetHandle(void)
{
	return netHandle_;
}