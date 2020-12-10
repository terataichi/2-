#pragma once
#include <memory>
#include "Object.h"
#include "../common/TileMap.h"

class BaseScene;

#define DETH_CNT_MAX 3000

class Bomb :
    public Object
{
public:
	Bomb(int& id, Vector2& pos,chronoTime& time,int length, BaseScene& scene);
	~Bomb();

	bool Update()override;
	void Draw(void)override;

	bool UpdateDef()override;						// ���͏����Ǘ�
	bool UpdateAuto()override;						// �I�[�g�p�C���b�g
	bool UpdateRev()override;						// ��M
	ObjectType ObjType() override { return ObjectType::Bomb; };

private:

	void AnimStateInit()override;

	BaseScene& scene_;

	chronoTime startTime_;

	int dethCnt_;
	int lengrh_;
};

