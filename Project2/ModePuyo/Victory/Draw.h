#pragma once
#include "../../Stage.h"

struct Draw
{
	bool operator()(Stage& stage)
	{
		//TRACE("ƒvƒŒƒCƒ„[ %d Draw\n", stage.id_ + 1);
		return true;
	}
};