#pragma once
#include <memory>
#include <Dxlib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()

class NetWork
{
public:
	static NetWork& GetInstance(void)
	{
		return *sInstance_;
	}

	IPDATA GetIP();															// ������IP�A�h���X���擾
	bool SetNetWorkMode(NetWorkMode mode);									// �l�b�g���[�N���[�h�̐ݒ�
	NetWorkMode GetNetWorkMode(void);										// �l�b�g���[�N���[�h�̎擾

	bool Update(void);														// �X�V
	bool GetActive(void);													// �ڑ��ł��Ă��邩�ǂ���
	bool GetReceiveData(Vector2& pos);										// �����Ă����f�[�^���󂯎��
	bool SetSendData(Vector2 pos);											// �f�[�^�𑗐M
	bool ConnectHost(IPDATA hostIP);										// �z�X�g�ɐڑ�
private:

	struct NetWorkDeleter
	{
		void operator() (NetWork* network)const
		{
			delete network;
		}
	};

	NetWork();
	~NetWork();

	static std::unique_ptr<NetWork, NetWorkDeleter> sInstance_;

	std::unique_ptr<NetWorkState> state_;									// �l�b�g���[�N�̏��
};

