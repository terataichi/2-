#pragma once
#include <thread>
#include <mutex>
#include <memory>
#include <map>
#include <functional>
#include <array>
#include <vector>
#include <Dxlib.h>
#include "NetWorkState.h"

#define HEADER_COUNT 2

#define lpNetWork NetWork::GetInstance()

enum class MesType:unsigned char
{
	NON = 100,						// ���M�����s�����Ƃ���0������P�[�X����������X�^�[�g�ʒu��ς���
	STANBY,							// ��������񑗐M
	GAME_START,						// �Q�[���J�n
	TMX_SIZE,						// TMX�t�@�C���̃T�C�Y
	TMX_DATA,						// TMX�e�[�^
	POS,
	SET_BOMB,
	DETH,							// ���S
	MAX
};

struct MesH
{
	MesType type;
	unsigned char  next;			// �f�[�^���M�𕪊�����ꍇ�ɁA�������邩�Ȃ���
	unsigned short sendID;			// �������̔ԍ�
	unsigned int length;			// �������ǂ����ɂ�����炸�A�P��̃f�[�^�̃T�C�Y
};

union UnionHeader
{
	MesH mesH;
	unsigned int iData[2];
};

union UnionData
{
	unsigned int uiData;
	int iData;
	char cData[4];
};

// �N���m�̌^��longlong�^�������̂ŕ����邱�Ƃɂ���
union TimeData
{
	std::chrono::system_clock::time_point time;
	int iData[2];
};

using ArrayIP = std::array<IPDATA, 5>;

using UnionVec = std::vector<UnionData>;

using MesTypeFunc = std::array<std::function<bool(MesH& data, UnionVec& packet)>, 7>;

using RevDataListP = std::vector<std::pair<MesH, UnionVec>>;

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

	void SetHeader(UnionHeader& header, UnionVec& vec);						// �w�b�_�[������������
	bool SendMes(MesType header,UnionVec data);								// ���M
	bool SendMes(MesType type);												// 

	bool ConnectHost(IPDATA hostIP);										// �z�X�g�ɐڑ�

	bool CheckConnect(void);												// �ڑ��ł��Ă��邩�m�F
	bool Update(void);														// �X�V
	bool CloseNetWork(void);
	void SendStanby(void);
	void SendStart(void);

	// �I�u�W�F�N�g����ǉ�����Ă����A�����Ŏ����̃f�[�^���Ǘ�����
	bool AddRevList(std::mutex& mtx, RevDataListP& data);
	
	//

	void RunUpDate(void);													// �}���`�X���b�h
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

	void InitFunc(void);													// �t�@���N�V�����������p
	void Init(void);

	static std::unique_ptr<NetWork, NetWorkDeleter> sInstance_;


	int tmxSize_;															// Tmx�t�@�C���T�C�Y�ۑ��p�ϐ� 
	UnionVec revBox_;

	std::vector<std::pair<std::mutex&,RevDataListP&>> revDataList_;			// ID�ɑ΂���󂯎�����f�[�^

	bool recvStanby_;														// ���������b�Z�[�W����M������ 
	std::unique_ptr<NetWorkState> state_;									// �l�b�g���[�N�̏��
	int tmp_;
	ArrayIP ip_;

	MesTypeFunc revUpdate_;													// ���b�Z�[�W�Ǘ��p�}�b�v

	std::chrono::system_clock::time_point  start_;
	std::chrono::system_clock::time_point  end_;

	std::thread update;

	unsigned int sendLength_;												// ���M�o�C�g��(�C���g�Ŋ���)
};

