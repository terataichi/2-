#include "Object.h"
int Object::count = 0;

Object::Object():id_(count)
{
	lpNetWork.AddRevList(revMutex_, revList_);
	rad_ = 0;
	count++;
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
