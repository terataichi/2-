#pragma once
#include "../Stage.h"

struct MunyonMode
{
	void operator()(Stage& stage)
	{
		stage.stgMode_ = StgMode::Rensa;
	}
};