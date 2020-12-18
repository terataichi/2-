#pragma once
#include <memory>
#include <map>
#include <functional>
#include <chrono>
#include "BaseScene.h"
#include "../common/Vector2.h"
#include "../Input/InputState.h"
#include "../NetWork/NetWorkState.h"
#include "../NetWork/NetWork.h"

#define SELECT_IP_MAX_X 3
#define SELECT_IP_MAX_Y 4

#define IPMAX 15

enum class UpdateMode
{
	SetNetWork,
	SetLastHostIP,
	StartInit,
	Play,
	SelectHostIP
};

enum class ChipLayer
{
	BG,
	ITEM,
	OBJ,
	CHAR
};

class TileMap;

class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();

	void Init(void) override;
	uniqueBase Update(uniqueBase scene) override;
	void DrawOwnScene()override;

private:

	void InitFunc(void);

	bool PlayUpdate(void);
	// 状態別アップデートの呼び出し先
	std::map<UpdateMode, std::function<bool(void)>> funcUpdate_;
	std::map<UpdateMode, std::function<void(void)>> funcDraw_;
	bool SetNetWorkMode(void);
	int SelectNetWorkMode(void);										// ネットワーク選択用
	bool SetLastHostIP(void);
	bool SetHostIP(std::string hostIPAddress);
	bool SelectHostIP(void);
	bool StartInit(void);

	void DrawSetNetWork(void);
	void DrawSetHostIP(void);
	void DrawStartInit(void);

	int screen_size_x_;
	int screen_size_y_;

	UpdateMode updateMode_;

	// player
	Vector2 pos_;
	int speed_;
	float rad_;

	bool wasHost_;														// 前回のホストに接続したいかどうか管理用
	std::unique_ptr<InputState> input_;									// キー入力用
	std::unique_ptr<InputState> numPad_;								// ナムパッド用

	std::shared_ptr<TileMap> tileMap_;

	int netWorkModeMaxSize_;											// 前回のIPがあるかどうかでサイズが変わる
	int netWorkModeTarget_;												// どのモードを選択しようとしているか												

	int selectIpTargetNum_;												// ターゲット計算用
	Vector2 selectIpTarget_;
	std::array<std::string, 12> targetNum_;								// ターゲットの場所によって文字が入っている		

	std::string hostIPAddress_;
	ArrayIP myIP_;

	bool timeOver_;
};

