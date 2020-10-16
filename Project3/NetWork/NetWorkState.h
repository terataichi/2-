#pragma once
#include <map>
#include <functional>
#include <DxLib.h>
#include "../common/Vector2.h"

#include "../_debug/_DebugConOut.h"
// ネットワークのモード用
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GUEST,
	MAX
};

enum class ActiveState
{
	Non,						// 未設定
	Wait,						// 接続待ち状態(ホスト用)
	Init,						// 初期化(ゲーム開始準備中、ホスト/ゲスト兼用)
	Stanby,						// 初期化済みの開始待ち(ホスト用)
	Play,						// ゲーム中(ホスト/ゲスト兼用)
	OFFLINE,
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();

	virtual NetWorkMode GetMode(void) { return NetWorkMode::OFFLINE; }		// モードの取得
	ActiveState GetActive(void);
	void SetActive(ActiveState state);										// 状態の更新
	bool Update(void);														// 更新
	bool CheckNetWork(void);												// 接続が切れていないかの確認

	int GetNetHandle(void);													// ネットハンドルの取得

	virtual bool CheckConnect(void) { return false; };						// 接続されたかの確認(ホスト用)
	virtual bool ConnectHost(IPDATA hostIP) { return false; };				// ホストに接続する(ゲスト:ホストは待機する)
protected:
	const int portNum_ = 8086;												// ポート番号格納変数(番号は基本的に何でもいい)
	ActiveState active_;
	int netHandle_ = 0;														// ０以上：確立した接続を示すネットワークハンドル(int型の識別値)格納用
};

