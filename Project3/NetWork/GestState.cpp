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

bool GuestState::SetSendData(Vector2 pos)
{
	ReaceiveData tmpPos;
	tmpPos = { pos.x,pos.y };
	NetWorkSend(netHandle_, &tmpPos, sizeof(ReaceiveData));
	return true;
}
