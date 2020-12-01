#include "GestState.h"
#include "NetWork.h"

GuestState::GuestState()
{
	active_ = ActiveState::Non;
}

GuestState::~GuestState()
{
}

bool GuestState::ConnectHost(IPDATA hostIP)
{
	// �O�ȏ�F�m�������ڑ��������l�b�g���[�N�n���h��(int�^�̎��ʒl)
	int netHandle = ConnectNetWork(hostIP, portNum_);

	// �ڑ��o�����玟�ɍs��
	if (0 <= netHandle)
	{
		listIntP& list = lpNetWork.GetHandleList();
		list.emplace_back(netHandle, -1);
		active_ = ActiveState::Init;
		return true;
	}
	return false;
}
