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
			if (!stage.ojamaFlg)
			{
				if (stage.ojamaList_.size())
				{
					int count = 0;								// �ő剽�~�点�邩
					for (auto ojama : stage.ojamaList_)
					{
						ojama->SetSpeed(16, 0);

						ojama->SetFall(true);

						stage.puyoVec_.emplace(stage.puyoVec_.begin(), ojama);
						count++;
						if (count >= 30)
						{
							break;
						}
					}
					stage.ojamaList_.erase(stage.ojamaList_.begin(), std::next(stage.ojamaList_.begin(), count));
					stage.ojamaFlg = true;
					stage.stgMode_ = StgMode::FALL;
					return;
				}
			}

			stage.ojamaFlg = false;
			stage.ResetRensa();							// �A���̃��Z�b�g
			stage.InstancePuyo();
			stage.CheckMove(stage.puyoVec_[1]);
			stage.CheckMove(stage.puyoVec_[0]);
			stage.stgMode_ = StgMode::Drop;
		}
	}
};