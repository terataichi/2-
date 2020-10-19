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
	// �ڑ����؂�Ă��Ȃ����`�F�b�N
	return CheckNetWork();
}

bool NetWorkState::CheckNetWork(void)
{
	if (GetLostNetWork() != -1)
	{
		// �Đڑ��̊J�n
		TRACE("�ڑ����؂�܂����A�Đڑ����܂�\n");
		return false;
	}
	return true;
}

int NetWorkState::GetNetHandle(void)
{
	return netHandle_;
}