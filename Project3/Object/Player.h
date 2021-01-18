#pragma once
#include <map>
#include <memory>
#include <list>
#include <functional>
#include "../common/Vector2.h"
#include "../common/TileMap.h"
#include "../Input/InputState.h"
#include "../DIR.h"
#include "Object.h"
#include "../Scene/BaseScene.h"

#define LENGTH_MAX 6
#define BOMB_MAX 4
#define SPEED_MAX 8

#define CHIP_SIZE 32
#define CHIP_RADIUS CHIP_SIZE / 2

using MoveFuncInput = std::function<bool(TrgData, bool)>;

using MoveFuncAuto = std::function<bool(DIR, bool)>;

//�z�X�gdef
class Player:
	public Object
{
public:
	Player(Vector2& pos, BaseScene& baseScene, LayerVec& layer, int id);

	~Player();

	bool Update()override;
	void Draw(void)override;
	
	bool CheckWallAuto(void);
	bool CheckWallInput(DIR dir);
	bool CheckMoveBombAuto(void);
	bool CheckMoveBomb(DIR dir);									// �{�����蔲���h�~

	bool UpdateDef()override;										// ���͏����Ǘ�
	bool UpdateAuto()override;										// �I�[�g�p�C���b�g
	bool UpdateRev()override;										// ��M

	void AddBombList(int no);
	int CheckBomb();

	static int lostCnt_;
	ObjectType ObjType() override { return ObjectType::Player; };
private:
	void ItemInitFunc(void);
	void InitFunc(void);											// �t�@���N�V�����̏�����

	std::list<int> bombList_;										// �g����{�����X�g
	std::map<DIR, Vector2> dirMap_;									// �L�[������

	std::array<DIR, 5> nextDir_;									// ����DIR���i�[
	int dirCnt_;

	std::function<bool(void)>netFunc_;								// ���������M���鑤�Ȃ̂��ǂ����ŏ������ς��

	std::unique_ptr<InputState> input_;

	BaseScene& scene_;												// �Q�[���V�[���A�N�Z�X�p

	std::map<ItemType, std::function<void(void)>> itemUpdate;
	std::list<MoveFuncInput>inputMoveList_;

	std::list<MoveFuncAuto>autoMoveList_;
	
	LayerVec& layerData_;

	int bombCnt_;
	int length_;													// �{���ɏ��𑗂�̂ɕK�v
	bool endFlg = false;
};

