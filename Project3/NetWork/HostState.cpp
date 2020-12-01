#include <DxLib.h>
#include <chrono>
#include "HostState.h"
#include "NetWork.h"

HostState::HostState()
{
	//active_ = !PreparationListenNetWork(portNum_);
	bool flg;
	flg = PreparationListenNetWork(portNum_) == 0 ? true : false;

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
	if (startFlg_)
	{
		return true;
	}

	int handle = GetNewAcceptNetWork();

	if(handle != -1)
	{
		// �ڑ�����Ă�̂ł���ȏ�ڑ�����Ȃ��悤�Ɏ~�߂�
		StopListenNetWork();
		// ��������Ԃɓ���
		handleList_.emplace_front(handle, -1);

		lpNetWork.SetCountDownFlg(true);

		startTime_ = lpSceneMng.GetTime();
		lpNetWork.SetStartTime(startTime_);
		TimeData time{ startTime_ };
		UnionData data[2];

		data[0].iData = time.iData[0];
		data[1].iData = time.iData[1];
		lpNetWork.SendMes(MesType::COUNT_DOWN_ROOM, UnionVec{ data[0] ,data[1] });
		connectCnt_++;
		TRACE("�ڑ����m�F����܂���\n");
	}

	if (lpNetWork.GetCountDownFlg() && !startFlg_)
	{
		chronoTime now = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - lpNetWork.GetStartTime()).count();

		if ((COUNT_DOWN_MAX - time) / 1000 <= 0)
		{
			startFlg_ = true;
			lpNetWork.SetPlayerMax(connectCnt_ + 1);		// �����̕�+1����
			active_ = ActiveState::Init;

			UnionData data[2];
			TimeData time{ now };

			data[0].iData = time.iData[0];
			data[1].iData = time.iData[1];

			lpNetWork.SetStartTime(time.time);
			lpNetWork.SendMesAll(MesType::COUNT_DOWN_GAME, UnionVec{ data[0],data[1] }, 0);
			return true;
		}
	}


	return false;
}
