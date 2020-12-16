#include "Object.h"

Object::Object()
{
	rad_ = 0;
	alive_ = true;
	dir_ = DIR::DOWN;
	state_ = STATE::Non;
	animCnt_ = 0;
	zOrder_ = 0;
	id_ = 0;
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

void Object::SetAlive(bool flg)
{
	alive_ = flg;
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
