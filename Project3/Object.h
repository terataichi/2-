#pragma once
#include"common/Vector2.h"
#include "NetWork/NetWork.h"
#include "common/TileMap.h"

class Object
{
public:
	Object();
	virtual ~Object() = default;

	bool CheckData(MesType type);
	virtual bool Update(LayerVec&& layer) = 0;
	virtual void Draw(void) = 0;

protected:

	std::mutex revMutex_;
	RevDataListP revList_;

	void PickData(MesType type, UnionVec& vec);

	Vector2 pos_;
	float rad_;
	Vector2 vel_;
	static int count;
	const int id_;
};

