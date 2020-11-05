#pragma once
#include <thread>
#include <mutex>
#include <memory>
#include <map>
#include <functional>
#include <array>
#include <vector>
#include <Dxlib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()

enum class MesType:unsigned char
{
	STANBY,							// 初期化情報送信
	GAME_START,						// ゲーム開始
	TMX_SIZE,						// TMXファイルのサイズ
	TMX_DATA,						// TMXテータ
	POS
};

struct TmxDataSize
{
	int sendNum;					// 何回データを送るか
	int oneSize;					// 一回で送るデータ量
	int allSize;					// 合計で送られる量
};

struct MesH
{
	MesType type;
	unsigned short id;				// 何番目の情報化
	unsigned char  cdata;
	unsigned int length;			// データのサイズ
	int data[2];
};

union UnionData
{
	char cData[8];
	int iData[2];
	long long IData;
};


using ArrayIP = std::array<IPDATA, 5>;

using TmxVec = std::vector<UnionData>;

using MesTypeFunc = std::map<MesType, std::function<bool(MesH& data)>>;

// 送るデータ
//struct MesData
//{
//	MesType type;
//	int data[2];
//};

class NetWork
{
public:
	static NetWork& GetInstance(void)
	{
		return *sInstance_;
	}

	ArrayIP GetMyIP(void);													// 自分のIPアドレスを取得
	bool SetNetWorkMode(NetWorkMode mode);									// ネットワークモードの設定
	NetWorkMode GetNetWorkMode(void);										// ネットワークモードの取得
	ActiveState GetActive(void);											// 接続先のステータス確認用
	bool SendMes(MesH& data);											// データを送信
	bool ConnectHost(IPDATA hostIP);										// ホストに接続

	bool CheckConnect(void);												// 接続できているか確認
	bool Update(void);														// 更新
	bool CloseNetWork(void);
	void SendStanby(void);
	void SendStart(void);

	void RunUpDate(void);													// マルチスレッド
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

	void InitFunc(void);													// ファンクション初期化用

	static std::unique_ptr<NetWork, NetWorkDeleter> sInstance_;


	int tmxSize_;															// Tmxファイルサイズ保存用変数 
	TmxVec RevBox;

	bool recvStanby_;														// 初期化メッセージを受信した証 
	std::unique_ptr<NetWorkState> state_;									// ネットワークの状態
	int tmp_;
	ArrayIP ip_;

	MesTypeFunc guestRevMap_;												// メッセージ管理用マップ
	MesTypeFunc hostRevMap_;												// メッセージ管理用マップ

	std::chrono::system_clock::time_point  start;
	std::chrono::system_clock::time_point  end;


	//std::thread update_;
	std::mutex lock_;
};

