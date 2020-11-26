#pragma once
#include <memory>
#include <map>
#include <functional>
#include <chrono>
#include "BaseScene.h"
#include "../common/Vector2.h"
#include "../Input/InputState.h"
#include "../NetWork/NetWorkState.h"

enum class UpdateMode
{
	SetNetWork,
	SetHostIP,
	StartInit,
	Play,
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
	bool PlayUpdate(void);
	// ��ԕʃA�b�v�f�[�g�̌Ăяo����
	std::map<UpdateMode, std::function<bool(void)>> func_;
	bool SetNetWorkMode(void);
	bool SetHostIP(void);
	bool StartInit(void);

	int screen_size_x_;
	int screen_size_y_;

	UpdateMode updateMode_;

	// player
	Vector2 pos_;
	int speed_;
	float rad_;

	bool wasHost_;											// �O��̃z�X�g�ɐڑ����������ǂ����Ǘ��p
	std::unique_ptr<InputState> input_;

	std::shared_ptr<TileMap> tileMap_;

	bool flg_ = false;

	//std::map<std::string, ChipLayer> chipLayer_;
	//std::map<ChipLayer, std::vector<int>>chipData_;
};

