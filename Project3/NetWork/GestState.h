#pragma once
#include "NetWorkState.h"
class GuestState :
	public NetWorkState
{
public:
	GuestState();
	~GuestState();

	NetWorkMode GetMode(void) override{ return NetWorkMode::GUEST; }	// ���[�h�̎擾
	bool ConnectHost(IPDATA hostIP) override;							// �z�X�g�ɐڑ�
private:

};

