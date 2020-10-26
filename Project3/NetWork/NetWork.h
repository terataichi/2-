#pragma once
#include <memory>
#include <array>
#include <vector>
#include <Dxlib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()

using ArrayIP = std::array<IPDATA, 5>;

using TmxVec = std::vector<int>;

enum class MesType
{
	STANBY,				// ��������񑗐M
	GAME_START,			// �Q�[���J�n
	TMX_SIZE,			// TMX�t�@�C���̃T�C�Y
	TMX_DATA,			// TMX�e�[�^
	POS
};

// ����f�[�^
struct MesData
{
	MesType type;
	int data[2];
};

class NetWork
{
public:
	static NetWork& GetInstance(void)
	{
		return *sInstance_;
	}

	ArrayIP GetMyIP(void);													// ������IP�A�h���X���擾
	bool SetNetWorkMode(NetWorkMode mode);									// �l�b�g���[�N���[�h�̐ݒ�
	NetWorkMode GetNetWorkMode(void);										// �l�b�g���[�N���[�h�̎擾
	ActiveState GetActive(void);											// �ڑ���̃X�e�[�^�X�m�F�p
	bool SendMes(MesData& data);											// �f�[�^�𑗐M
	bool ConnectHost(IPDATA hostIP);										// �z�X�g�ɐڑ�

	bool CheckConnect(void);												// �ڑ��ł��Ă��邩�m�F
	bool Update(void);														// �X�V
	bool CloseNetWork(void);
	void SendStanby(void);
	void SendStart(void);

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


	int tmxSize_;															// Tmx�t�@�C���T�C�Y�ۑ��p�ϐ� 
	TmxVec RevBox;

	bool recvStanby_;														// ���������b�Z�[�W����M������ 
	std::unique_ptr<NetWorkState> state_;									// �l�b�g���[�N�̏��
	int tmp_;
};

