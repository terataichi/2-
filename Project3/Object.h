#pragma once
#include"common/Vector2.h"
#include "NetWork/NetWork.h"
#include "common/TileMap.h"

#define UNIT_ID_BASE 5			// �{�����o����ő吔

class Object
{
public:
	Object();
	virtual ~Object() = default;

	const int ID(void) { return id_; };
	bool CheckData(MesType type);
	virtual bool Update(LayerVec&& layer) = 0;
	virtual void Draw(void) = 0;

	virtual bool UpdateDef(LayerVec& layer) = 0;					// ���͏����Ǘ�
	virtual bool UpdateAuto(LayerVec& layer) = 0;					// �I�[�g�p�C���b�g
	virtual bool UpdateRev(LayerVec& layer)= 0;						// ��M

protected:

	std::mutex revMutex_;
	RevDataListP revList_;

	void PickData(MesType type, UnionVec& vec);

	Vector2 pos_;
	float rad_;
	Vector2 vel_;
	static int count;
	int id_;
	bool alive_;
};

