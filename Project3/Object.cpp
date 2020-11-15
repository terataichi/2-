#include "Object.h"

Object::Object():id_()
{
	lpNetWork.AddRevList(revList_);
}

Object::~Object()
{
}

UnionVec Object::PickData(MesType type)
{
	int cnt = 0;
	for (auto& data : revList_)
	{
		if (data.first.type == type)
		{
			auto vec = data.second;
			revList_.erase(revList_.begin() + cnt);
			return vec;
		}
		cnt++;
	}

	return UnionVec{};
}
