#include <DxLib.h>
#include <chrono>
#include "HostState.h"
#include "NetWork.h"

HostState::HostState()
{
	//active_ = !PreparationListenNetWork(portNum_);
	bool flg;
	flg = PreparationListenNetWork(portNum_) == 0 ? true : false;
	initFlg_ = false;
	active_ = ActiveState::Non;
	connectCnt_ = 0;
	if (flg)
	{
		// �ڑ��҂��ɓ���
		active_ = ActiveState::Wait;
	}
}

HostState::~HostState()
{
}

//ActiveState HostState::CheckPreparation(void)
//{
//	bool flg;
//	flg = PreparationListenNetWork(portNum_) == 0 ? true : false;
//
//	// �ڑ��҂���Ԃɂ���
//	if (flg) return ActiveState::Wait;
//
//	return ActiveState::Non;
//}

bool HostState::CheckConnect(void)
{
	if (lpNetWork.GetStartCntFlg())
	{
		return true;
	}

	int handle = GetNewAcceptNetWork();

	if (handle != -1)
	{

		// ��������Ԃɓ���
		lpNetWork.AddHandleList({ handle, -1 });

		// �J�E���g�_�E���̑��M
		if (!lpNetWork.GetCountDownFlg())
		{
			lpNetWork.SetStartTime(lpSceneMng.GetTime());
			lpNetWork.SetCountDownFlg(true);
		}
		TimeData time{ lpNetWork.GetStartTime() };
		UnionData data[2];

		data[0].iData = time.iData[0];
		data[1].iData = time.iData[1];
		lpNetWork.SendMes(MesType::COUNT_DOWN_ROOM, UnionVec{ data[0] ,data[1] });
		connectCnt_++;
		TRACE("�ڑ����m�F����܂���\n");
	}

	if (lpNetWork.GetCountDownFlg() && !initFlg_)
	{
	
		chronoTime now = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - lpNetWork.GetStartTime()).count();

		if ((COUNT_DOWN_MAX - time) / 1000 <= 0)
		{
			lpNetWork.SetPlayerMax(connectCnt_ + 1);		// �����̕�+1����
			active_ = ActiveState::Init;
			initFlg_ = true;
			//�@ID�̐U�蕪��
			auto& handlelist = lpNetWork.GetHandleList();
			int max = static_cast<int>(handlelist.size());
			int cnt = max;

			UnionData idData[2];
			idData[1].iData = max + 1;
			for (auto& list : handlelist)
			{
				list.id_ = cnt * 5;
				idData[0].iData = list.id_;
				cnt--;
				lpNetWork.SendMes(MesType::ID, UnionVec{ idData[0] ,idData[1] }, list.handle_);
			}
			// �ڑ�����Ă�̂ł���ȏ�ڑ�����Ȃ��悤�Ɏ~�߂�
			StopListenNetWork();
			return true;
		}
	}


	return false;
}
