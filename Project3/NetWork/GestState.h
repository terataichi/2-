#pragma once
#include "NetWorkState.h"
class GuestState :
	public NetWorkState
{
public:
	GuestState();
	~GuestState();

	NetWorkMode GetMode(void) override{ return NetWorkMode::GUEST; }		// モードの取得
	ActiveState ConnectHost(IPDATA hostIP) override;						// ホストに接続
	bool SetSendData(Vector2 pos)override;									// ホストにデータを送る
private:

};

