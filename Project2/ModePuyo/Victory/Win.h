#pragma once
#include "../../Stage.h"

struct Win
{
	bool operator()(Stage& stage)
	{
		//TRACE("�v���C���[ %d Win\n",stage.id_ + 1);
		return true;
	}
};