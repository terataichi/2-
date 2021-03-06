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
	//connectCnt_ = 0;
	if (flg)
	{
		// 接続待ちに入る
		active_ = ActiveState::Wait;
	}
}

HostState::~HostState()
{
}

bool HostState::CheckConnect(void)
{
	if (lpNetWork.GetStartCntFlg())
	{
		return true;
	}

	int handle = GetNewAcceptNetWork();

	if (handle != -1)
	{

		// 初期化状態に入る
		lpNetWork.AddHandleList({ handle, -1 });
		lpNetWork.AddPlayerMax();

		// カウントダウンの送信
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
		TRACE("接続が確認されました\n");
	}

	if (lpNetWork.GetCountDownFlg() && !initFlg_)
	{
		chronoTime now = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - lpNetWork.GetStartTime()).count();
		if ((COUNT_DOWN_MAX - time) / 1000 < 0)
		{
			lpNetWork.AddPlayerMax();
			active_ = ActiveState::Init;
			initFlg_ = true;
			//　IDの振り分け
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
				TRACE("IDの送信\n");
				lpNetWork.SendMes(MesType::ID, UnionVec{ idData[0] ,idData[1] }, list.handle_);
			}
			// 接続されてるのでこれ以上接続されないように止める
			StopListenNetWork();
			return true;
		}
	}

	// 初期化情報がすでに送っあるから
	if (initFlg_)
	{
		return true;
	}

	return false;
}
