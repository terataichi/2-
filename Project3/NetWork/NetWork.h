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

#define HEADER_COUNT 2

#define lpNetWork NetWork::GetInstance()

enum class MesType:unsigned char
{
	NON = 100,						// 送信が失敗したときに0が来るケースが多いからスタート位置を変える
	STANBY,							// 初期化情報送信
	GAME_START,						// ゲーム開始
	TMX_SIZE,						// TMXファイルのサイズ
	TMX_DATA,						// TMXテータ
	POS
};

struct MesH
{
	MesType type;
	unsigned char  next;			// データ送信を分割する場合に、次があるかないか
	unsigned short sendID;			// 分割時の番号
	unsigned int length;			// 分割かどうかにかかわらず、単一のデータのサイズ
};

union UnionHeader
{
	MesH mesH;
	unsigned int iData[2];
};

union UnionData
{
	unsigned int uiData;
	int iData;
	char cData[4];
};


using ArrayIP = std::array<IPDATA, 5>;

using UnionVec = std::vector<UnionData>;

using MesTypeFunc = std::map<MesType, std::function<bool(MesH& data, UnionVec& packet)>>;

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

	void SetHeader(UnionHeader header, UnionVec& vec);						// ヘッダー部をくっつける
	bool SendMes(MesType header,UnionVec data);								// 送信
	bool SendMes(MesType type);												// 

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
	void Init(void);

	static std::unique_ptr<NetWork, NetWorkDeleter> sInstance_;


	int tmxSize_;															// Tmxファイルサイズ保存用変数 
	UnionVec revBox_;

	bool recvStanby_;														// 初期化メッセージを受信した証 
	std::unique_ptr<NetWorkState> state_;									// ネットワークの状態
	int tmp_;
	ArrayIP ip_;

	MesTypeFunc guestRevMap_;												// メッセージ管理用マップ
	MesTypeFunc hostRevMap_;												// メッセージ管理用マップ

	std::chrono::system_clock::time_point  start_;
	std::chrono::system_clock::time_point  end_;

	std::thread update;

	unsigned int sendLength_;												// 送信バイト長(イントで割る)

	//std::thread update_;
	std::mutex lock_;
};

