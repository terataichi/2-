#pragma once
#include "../Stage.h"

struct MunyonMode
{
	void operator()(Stage& stage)
	{
		bool flg = true;

		// �S���ނɂ��I����Ă邩�`�F�b�N
		for (auto puyo : stage.puyoVec_)
		{
			flg &= puyo->CheckMunyon();
		}

		if (flg)
		{
			stage.InstancePuyo();
			stage.CheckMove(stage.puyoVec_[1]);
			stage.CheckMove(stage.puyoVec_[0]);
			stage.stgMode_ = StgMode::Drop;
		}
	}
};