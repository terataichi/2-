#pragma once
#include <map>
#include <functional>
#include <DxLib.h>
#include "../common/Vector2.h"

#include "../_debug/_DebugConOut.h"
// �l�b�g���[�N�̃��[�h�p
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GUEST,
	MAX
};

enum class ActiveState
{
	Non,						// ���ݒ�
	Wait,						// �ڑ��҂����(�z�X�g�p)
	Init,						// ������(�Q�[���J�n�������A�z�X�g/�Q�X�g���p)
	Stanby,						// �������ς݂̊J�n�҂�(�z�X�g�p)
	Play,						// �Q�[����(�z�X�g/�Q�X�g���p)
	OFFLINE,
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();

	virtual NetWorkMode GetMode(void) { return NetWorkMode::OFFLINE; }		// ���[�h�̎擾
	ActiveState GetActive(void);
	void SetActive(ActiveState state);										// ��Ԃ̍X�V
	bool Update(void);														// �X�V
	bool CheckNetWork(void);												// �ڑ����؂�Ă��Ȃ����̊m�F

	int GetNetHandle(void);													// �l�b�g�n���h���̎擾

	virtual bool CheckConnect(void) { return false; };						// �ڑ����ꂽ���̊m�F(�z�X�g�p)
	virtual bool ConnectHost(IPDATA hostIP) { return false; };				// �z�X�g�ɐڑ�����(�Q�X�g:�z�X�g�͑ҋ@����)
protected:
	const int portNum_ = 8086;												// �|�[�g�ԍ��i�[�ϐ�(�ԍ��͊�{�I�ɉ��ł�����)
	ActiveState active_;
	int netHandle_ = 0;														// �O�ȏ�F�m�������ڑ��������l�b�g���[�N�n���h��(int�^�̎��ʒl)�i�[�p
};

