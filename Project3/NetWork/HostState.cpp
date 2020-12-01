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

	if (flg)
	{
		// 接続待ちに入る
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
//	// 接続待ち状態にする
//	if (flg) return ActiveState::Wait;
//
//	return ActiveState::Non;
//}

bool HostState::CheckConnect(void)
{
	int handle = GetNewAcceptNetWork();

	if(handle != -1)
	{
		// 接続されてるのでこれ以上接続されないように止める
		StopListenNetWork();
		// 初期化状態に入る
		handleList_.emplace_front(handle, -1);

		lpNetWork.SetCountDownFlg(true);
		startTime_ = lpSceneMng.GetTime();
		TimeData time{ startTime_ };
		UnionData data[2];

		data[0].iData = time.iData[0];
		data[1].iData = time.iData[1];
		lpNetWork.SendMes(MesType::COUNT_DOWN_ROOM, UnionVec{ data[0] ,data[1] });

		TRACE("接続が確認されました\n");
	}

	if (lpNetWork.GetCountDownFlg())
	{
		chronoTime now = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count();

		if ((COUNT_DOWN_MAX - time) / 1000 <= 0)
		{
			active_ = ActiveState::Init;
			return true;
		}
	}


	return false;
}
