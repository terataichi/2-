#pragma once
#include "NetWorkState.h"
class GuestState :
	public NetWorkState
{
public:
	GuestState();
	~GuestState();

	NetWorkMode GetMode(void) override{ return NetWorkMode::GUEST; }	// モードの取得
	bool ConnectHost(IPDATA hostIP) override;							// ホストに接続
private:

};

