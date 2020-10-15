#pragma once
#include "../_debug/_DebugConOut.h"
#include <DxLib.h>
#include "../common/Vector2.h"

// ネットワークのモード用
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GUEST,
	MAX
};

struct ReaceiveData
{
	int x,y;
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();

	virtual NetWorkMode GetMode(void) { return NetWorkMode::OFFLINE; }		// モードの取得
	bool GetActive(void);
	bool Update(void);														// 更新
	virtual bool CheckConnect(void);										// 接続の確認
	virtual bool ConnectHost(IPDATA hostIP);								// ホストに接続
	virtual bool GetReceiveData(Vector2&);									// データの受け取り
	virtual bool SetSendData(Vector2 pos);											// データを送る
protected:
	const int portNum_ = 8086;												// ポート番号格納変数(番号は基本的に何でもいい)
	bool active_;
	int netHandle_ = 0;														// ０以上：確立した接続を示すネットワークハンドル(int型の識別値)格納用
};

