#include "GestState.h"

GuestState::GuestState()
{
	active_ = ActiveState::Non;
}

GuestState::~GuestState()
{
}

ActiveState GuestState::ConnectHost(IPDATA hostIP)
{
	// �O�ȏ�F�m�������ڑ��������l�b�g���[�N�n���h��(int�^�̎��ʒl)
	netHandle_ = ConnectNetWork(hostIP, portNum_);

	// �ڑ��o�����玟�ɍs��
	if (0 <= netHandle_)
	{
		return ActiveState::Init;
	}
	return  ActiveState::Non;
}

bool GuestState::SetSendData(Vector2 pos)
{
	ReaceiveData tmpPos;
	tmpPos = { pos.x,pos.y };
	NetWorkSend(netHandle_, &tmpPos, sizeof(ReaceiveData));
	return true;
}
