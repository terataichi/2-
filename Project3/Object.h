#pragma once
#include"common/Vector2.h"
#include "NetWork/NetWork.h"

class Object
{
public:
	Object();
	virtual ~Object();

	

protected:

	RevDataListP revList_;

	UnionVec PickData(MesType type);

	Vector2 pos_;
	float rad_;
	Vector2 vel_;
	const int id_;
};

