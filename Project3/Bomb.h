#pragma once
#include "Object.h"

class BaseScene;

class Bomb :
    public Object
{
public:
	Bomb(int& id, Vector2& pos, BaseScene& scene);
	~Bomb();

	bool Update(LayerVec&& layer)override;
	void Draw(void)override;

	bool UpdateDef(LayerVec& layer)override;						// ���͏����Ǘ�
	bool UpdateAuto(LayerVec& layer)override;						// �I�[�g�p�C���b�g
	bool UpdateRev(LayerVec& layer)override;						// ��M

private:
	BaseScene& scene_;


	int dethCnt_;
};

