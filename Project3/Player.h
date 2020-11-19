#pragma once
#include <map>
#include <memory>
#include <list>
#include <functional>
#include "common/Vector2.h"
#include "common/TileMap.h"
#include "Input/InputState.h"
#include "DIR.h"
#include "Object.h"
#include "Scene/BaseScene.h"

//�z�X�gdef
class Player:
	public Object
{
public:
	Player(Vector2& pos,BaseScene& baseScene);

	~Player();

	bool Update(LayerVec&& layer)override;
	void Draw(void)override;
	
	bool CheckWall(LayerVec& layer);

	bool UpdateDef(LayerVec& layer)override;						// ���͏����Ǘ�
	bool UpdateAuto(LayerVec& layer)override;						// �I�[�g�p�C���b�g
	bool UpdateRev(LayerVec& layer)override;						// ��M

	void AddBombList(int no);
	int CheckBomb();

	static int lostCont_;

private:
	DIR dir_;

	std::list<int> bombList_;								// �g����{�����X�g
	std::map<DIR, Vector2> dirMap_;							// �L�[�������ňړ��ʂ�����

	std::function<bool(LayerVec&)>netFunc_;

	std::unique_ptr<InputState> input_;

	BaseScene& scene_;

	int animCnt_;
};

