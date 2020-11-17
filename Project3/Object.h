#pragma once
#include"common/Vector2.h"
#include "NetWork/NetWork.h"

class Object
{
public:
	Object();
	virtual ~Object();

	

protected:

	std::mutex revMutex_;
	RevDataListP revList_;

	void PickData(MesType type, UnionVec& vec);
	bool CheckData(MesType type);

	Vector2 pos_;
	float rad_;
	Vector2 vel_;
	static int count;
	const int id_;
};

