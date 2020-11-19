#include "Object.h"
int Object::count = 0;

Object::Object():id_(count)
{
	lpNetWork.AddRevList(revMutex_, revList_);
	rad_ = 0;
	count += 5;
}

void Object::PickData(MesType type,UnionVec& vec)
{
	std::lock_guard<std::mutex> lock(revMutex_);
	int cnt = 0;
	for (auto& data : revList_)
	{
		if (data.first.type == type)
		{
			vec = data.second;
			revList_.erase(revList_.begin() + cnt);
			break;
		}
		cnt++;
	}
}

bool Object::CheckData(MesType type)
{
	std::lock_guard<std::mutex> lock(revMutex_);

	for (auto& data : revList_)
	{
		if (data.first.type == type)
		{
			return true;
		}
	}
	return false;
}
