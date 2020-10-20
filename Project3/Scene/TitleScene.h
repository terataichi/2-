#pragma once
#include <memory>
#include <map>
#include <functional>
#include "BaseScene.h"
#include "../common/Vector2.h"
#include "../Input/InputState.h"
#include "../NetWork/NetWorkState.h"


struct TileMap
{
	std::string version;
	std::string tiledversion;
	std::string orientation;
	std::string	renderorder;			// enumとかに変えたい
	int width;							// タイルのマスの数(横)
	int height;							// 　　　 "　　　　(縦)
	int tileWidth;						// 1タイルのサイズ(横)
	int tileHeight;						//　　　 "　　　　(縦)
	int infinite;
	int nextLayerID;					// layerの数
	int nextObjectID;
};

enum class UpdateMode
{
	SetNetWork,
	SetHostIP,
	StartInit,
	Play,
};

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Init(void) override;
	uniqueBase Update(uniqueBase scene) override;
	void Draw() override;

private:
	void PlayUpdate(void);
	void SetNetWorkMode(void);
	void SetHostIP(void);
	void StartInit(void);
	std::map<UpdateMode, std::function<void(void)>> func_;

	int screen_size_x_;
	int screen_size_y_;

	UpdateMode updateMode_;

	// player
	Vector2 pos_;
	int speed_;
	float rad_;

	bool wasHost_;											// 前回のホストに接続したいかどうか管理用
	std::unique_ptr<InputState> input_;
};

