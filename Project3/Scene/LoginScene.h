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
	// ��ԕʃA�b�v�f�[�g�̌Ăяo����
	std::map<UpdateMode, std::function<bool(void)>> funcUpdate_;
	std::map<UpdateMode, std::function<void(void)>> funcDraw_;
	bool SetNetWorkMode(void);
	int SelectNetWorkMode(void);										// �l�b�g���[�N�I��p
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

	bool wasHost_;														// �O��̃z�X�g�ɐڑ����������ǂ����Ǘ��p
	std::unique_ptr<InputState> input_;									// �L�[���͗p
	std::unique_ptr<InputState> numPad_;								// �i���p�b�h�p

	std::shared_ptr<TileMap> tileMap_;

	int netWorkModeMaxSize_;											// �O���IP�����邩�ǂ����ŃT�C�Y���ς��
	int netWorkModeTarget_;												// �ǂ̃��[�h��I�����悤�Ƃ��Ă��邩												

	int selectIpTargetNum_;												// �^�[�Q�b�g�v�Z�p
	Vector2 selectIpTarget_;
	std::array<std::string, 12> targetNum_;								// �^�[�Q�b�g�̏ꏊ�ɂ���ĕ����������Ă���		

	std::string hostIPAddress_;
	ArrayIP myIP_;

	bool timeOver_;
};

