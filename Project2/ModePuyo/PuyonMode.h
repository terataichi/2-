#pragma once
#include "../Stage.h"

struct PuyonMode
{
	void operator()(Stage& stage)
	{
		bool flg = true;
		Vector2 grid{ 0,0 };

		for (auto vec : stage.puyoVec_)
		{
			flg &= vec->CheckPuyon();
		}

		if (flg)
		{
			stage.stgMode_ = StgMode::Munyon;
		}
	}
};