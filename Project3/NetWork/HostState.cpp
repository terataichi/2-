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
	if (startFlg_)
	{
		return true;
	}

	int handle = GetNewAcceptNetWork();

	if (handle != -1)
	{
		// 接続されてるのでこれ以上接続されないように止める
		StopListenNetWork();
		// 初期化状態に入る
		lpNetWork.AddHandleList({ handle, -1 });

		lpNetWork.SetCountDownFlg(true);

		startTime_ = lpSceneMng.GetTime();
		lpNetWork.SetStartTime(startTime_);
		TimeData time{ startTime_ };
		UnionData data[2];

		data[0].iData = time.iData[0];
		data[1].iData = time.iData[1];
		lpNetWork.SendMes(MesType::COUNT_DOWN_ROOM, UnionVec{ data[0] ,data[1] });
		connectCnt_++;
		TRACE("接続が確認されました\n");
	}

	if (lpNetWork.GetCountDownFlg() && !startFlg_)
	{
		chronoTime now = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - lpNetWork.GetStartTime()).count();

		if ((COUNT_DOWN_MAX - time) / 1000 <= 0)
		{
			startFlg_ = true;
			lpNetWork.SetPlayerMax(connectCnt_ + 1);		// 自分の分+1する
			active_ = ActiveState::Init;

			//　IDの振り分けと送信
			auto& handlelist = lpNetWork.GetHandleList();
			int cnt = static_cast<int>(handlelist.size() - 1);

			for (auto list : handlelist)
			{
				list.second = cnt * 5;
			}


			// カウントダウンの送信
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
