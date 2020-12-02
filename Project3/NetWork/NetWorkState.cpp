#include "NetWorkState.h"
#include "NetWork.h"

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
	CheckConnect();
	return CheckNetWork();
}

bool NetWorkState::CheckNetWork(void)
{
	if (GetLostNetWork() != -1)
	{
		// �Đڑ��̊J�n
		//handleList_.front().first = -1;
		TRACE("�ڑ����؂�܂����A�Đڑ����܂�\n");
		return false;
	}
	return true;
}

bool NetWorkState::SetPlayerID(int id)
{
	// ������ID���Z�b�g
	listIntP& list = lpNetWork.GetHandleList();
	list.front().id_ = id;
	return true;
}

//listIntP NetWorkState::GetNetHandle(void)
//{
//	return handleList_;
//}