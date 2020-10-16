#pragma once
#include <memory>
#include <Dxlib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()

enum class MesType
{
	STANBY,				// 初期化情報送信
	GAME_START,			// ゲーム開始
	POS
};

// 送るデータ
struct MesData
{
	MesType type;
	int data[2];
};

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
	bool CloseNetWork(void);
	ActiveState GetActive(void);											// 接続できているかどうか
	bool GetReceiveData(Vector2& pos);										// 送られてきたデータを受け取る
	void SendStanby(void);
	void SendStart(void);

	bool SendMes(MesData& data);											// データを送信
	ActiveState ConnectHost(IPDATA hostIP);									// ホストに接続
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

