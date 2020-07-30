#pragma once
#include "../Stage.h"

struct PuyonMode
{
	void operator()(Stage& stage)
	{
		bool flg = true;

		for (auto vec : stage.puyoVec_)
		{
			flg &= vec->CheckPuyon();
		}

		if (flg)
		{
			stage.stgMode_ = StgMode::Rensa;
		}
	}
};