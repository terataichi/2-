#pragma once
#include <thread>
#include <mutex>
#include <memory>
#include <map>
#include <functional>
#include <array>
#include <vector>
#include <Dxlib.h>
#include <chrono>
#include "NetWorkState.h"

#define HEADER_COUNT 2

#define lpNetWork NetWork::GetInstance()

#define COUNT_DOWN_MAX 15000
#define COUNT_START_MAX 10000

enum class MesType:unsigned char
{
	NON = 100,						// 送信が失敗したときに0が来るケースが多いからスタート位置を変える
	COUNT_DOWN_ROOM,				// 接続受付カウントダウン
	ID,								// 自分のIDとﾌﾟﾚｲﾔｰ総数
	STANBY_HOST,					// 初期化情報送信
	STANBY_GUEST,					// ゲーム開始
	COUNT_DOWN_GAME,				// 全員の初期化完了後のゲーム開始時間
	TMX_SIZE,						// TMXファイルのサイズ
	TMX_DATA,						// TMXテータ
	POS,							// 座標
	SET_BOMB,						// インスタンスするボムの情報
	DETH,							// 死亡
	RESULT,							// リザルト
	LOST,							// 切断時に生成
	MAX
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

// クロノの型がlonglong型だったので分けることに
union TimeData
{
	std::chrono::system_clock::time_point time;
	int iData[2];
};

using ArrayIP = std::array<IPDATA, 5>;

using UnionVec = std::vector<UnionData>;

using MesTypeFunc = std::array<std::function<bool(MesH& data, UnionVec& packet)>, static_cast<int>(MesType::MAX)>;

using RevDataListP = std::vector<std::pair<MesH, UnionVec>>;

using chronoTime = std::chrono::system_clock::time_point;

// 1:ネットハンドル, 2:ID
// 0
// 準備完了 1
// -1 切断
struct PlayerHandle
{
	int handle_;
	int id_;
	int state_;
};

using listIntP = std::list<PlayerHandle>;

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
	void SetActive(ActiveState state);

	void SetHeader(UnionHeader& header, UnionVec& vec);						// ヘッダー部をくっつける
	bool SendMes(MesType header,UnionVec data);								// 送信
	bool SendMes(MesType header, UnionVec data, int handle);				// ハンドル指定版
	bool SendMesAll(MesType header, UnionVec data, int handle);
	bool SendMes(MesType type);												// 
	bool SendMesAll(MesType type);

	bool ConnectHost(IPDATA hostIP);										// ホストに接続

	bool CheckConnect(void);												// 接続できているか確認
	bool Update(void);														// 更新
	bool CloseNetWork(void);
	void SendStanby(void);
	void SendStart(void);

	bool CheckNetWork();

	bool EndNetWork(void);

	chronoTime GetStartTime(void);											// 接続待ち開始時間取得用
	void SetStartTime(chronoTime time);
	bool GetCountDownFlg(void);												//
	void SetCountDownFlg(bool flg);
	void SetStartCntFlg(bool flg);
	bool GetStartCntFlg(void);												// スタートのカウントダウン開始していいか
	void AddPlayerMax();
	const int GetPlayerMax(void)const;
	const int GetPlayerID(void)const;
	const UnionVec& GetResultData(void)const;
	void AddHandleList(PlayerHandle intp);
	listIntP& GetHandleList(void);

	// オブジェクトから追加されていく、自分で自分のデータを管理する
	bool AddRevList(std::mutex& mtx, RevDataListP& data);
	
	//

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
	bool AddList(MesH& data, UnionVec& packet);

	static std::unique_ptr<NetWork, NetWorkDeleter> sInstance_;


	int tmxSize_;															// Tmxファイルサイズ保存用変数 
	UnionVec revBox_;
	UnionVec resultData_;

	std::vector<std::pair<std::mutex&,RevDataListP&>> revDataList_;			// IDに対する受け取ったデータ

	bool recvStanby_;														// 初期化メッセージを受信した証 
	std::unique_ptr<NetWorkState> state_;									// ネットワークの状態
	int tmp_;
	ArrayIP ip_;

	MesTypeFunc revUpdate_;													// メッセージ管理用マップ

	std::chrono::system_clock::time_point  start_;
	std::chrono::system_clock::time_point  end_;

	std::thread update_;

	unsigned int sendLength_;												// 送信バイト長(イントで割る)

	int playerID_;															// 自分のプレイヤーID
	int playerMax_;															// プレイヤーの最大人数
	chronoTime countDownTime_;
	bool countDownFlg_;														// カウントダウンが開始されているか
	bool startCntFlg_;
	int loadStage_;															// ｽﾃｰｼﾞの情報は一度しか送られてきてほしくない
	listIntP handlelist_;

	bool endFlg_;															// ネットワークを切断していいか
	int stanbyCnt_;

	std::map<MesType, int> mesTypeSize_;
};

