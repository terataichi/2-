#include "GestState.h"

GuestState::GuestState()
{
	active_ = false;
}

GuestState::~GuestState()
{
}

bool GuestState::ConnectHost(IPDATA hostIP)
{
	// �O�ȏ�F�m�������ڑ��������l�b�g���[�N�n���h��(int�^�̎��ʒl)
	netHandle_ = ConnectNetWork(hostIP, portNum_);
	return  active_ = (0 <= netHandle_);
}
