#pragma once
#include "NetWorkState.h"
class GuestState :
	public NetWorkState
{
public:
	GuestState();
	~GuestState();

	NetWorkMode GetMode(void) override{ return NetWorkMode::GUEST; }		// ���[�h�̎擾
	ActiveState ConnectHost(IPDATA hostIP) override;						// �z�X�g�ɐڑ�
	bool SetSendData(Vector2 pos)override;									// �z�X�g�Ƀf�[�^�𑗂�
private:

};

