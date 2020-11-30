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
		handleList_.front().first = -1;
		TRACE("接続が切れました、再接続します\n");
		return false;
	}
	return true;
}

bool NetWorkState::SetPlayerID(int id)
{
	// 自分のIDをセット
	handleList_.front().second = id;
	return true;
}

listIntP NetWorkState::GetNetHandle(void)
{
	return handleList_;
}