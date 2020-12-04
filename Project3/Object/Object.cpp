#include "Object.h"
int Object::count = 0;

Object::Object():id_(count)
{
	rad_ = 0;
	alive_ = true;
	count += UNIT_ID_BASE;
	dir_ = DIR::DOWN;
	state_ = STATE::Non;
	animCnt_ = 0;
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

void Object::AnimStateInit(void)
{
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
