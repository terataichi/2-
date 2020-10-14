#pragma once
#include "../_debug/_DebugConOut.h"
#include <DxLib.h>

// ネットワークのモード用
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GUEST,
	MAX
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();

	virtual NetWorkMode GetMode(void) { return NetWorkMode::OFFLINE; }		// モードの取得
	bool GetActive(void);
	virtual bool ConnectHost(IPDATA hostIP);								// ホストに接続
protected:
	const int portNum_ = 8086;												// ポート番号格納変数(番号は基本的に何でもいい)
	bool active_;
	int netHandle_ = 0;														// ０以上：確立した接続を示すネットワークハンドル(int型の識別値)格納用
};

