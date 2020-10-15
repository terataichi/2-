#pragma once
#include <memory>
#include <Dxlib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()

class NetWork
{
public:
	static NetWork& GetInstance(void)
	{
		return *sInstance_;
	}

	IPDATA GetIP();															// 自分のIPアドレスを取得
	bool SetNetWorkMode(NetWorkMode mode);									// ネットワークモードの設定
	NetWorkMode GetNetWorkMode(void);										// ネットワークモードの取得

	bool Update(void);														// 更新
	bool GetActive(void);													// 接続できているかどうか
	bool GetReceiveData(Vector2& pos);										// 送られてきたデータを受け取る
	bool SetSendData(Vector2 pos);											// データを送信
	bool ConnectHost(IPDATA hostIP);										// ホストに接続
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

	static std::unique_ptr<NetWork, NetWorkDeleter> sInstance_;

	std::unique_ptr<NetWorkState> state_;									// ネットワークの状態
};

