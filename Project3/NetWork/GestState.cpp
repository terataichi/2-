#include "GestState.h"

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
	netHandle_ = ConnectNetWork(hostIP, portNum_);

	// �ڑ��o�����玟�ɍs��
	if (0 <= netHandle_)
	{
		active_ = ActiveState::Init;
		return true;
	}
	return false;
}
