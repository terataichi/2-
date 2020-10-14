#pragma once
#include "../_debug/_DebugConOut.h"
#include <DxLib.h>

// �l�b�g���[�N�̃��[�h�p
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GUEST,
	MAX
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();

	virtual NetWorkMode GetMode(void) { return NetWorkMode::OFFLINE; }		// ���[�h�̎擾
	bool GetActive(void);
	virtual bool ConnectHost(IPDATA hostIP);								// �z�X�g�ɐڑ�
protected:
	const int portNum_ = 8086;												// �|�[�g�ԍ��i�[�ϐ�(�ԍ��͊�{�I�ɉ��ł�����)
	bool active_;
	int netHandle_ = 0;														// �O�ȏ�F�m�������ڑ��������l�b�g���[�N�n���h��(int�^�̎��ʒl)�i�[�p
};

