#pragma once
#include "../Stage.h"
#include <random>


struct MunyonMode
{
	void operator()(Stage& stage)
	{
		bool flg = true;

		// 全員むにょん終わってるかチェック
		for (auto puyo : stage.puyoVec_)
		{
			flg &= puyo->CheckMunyon();
		}

		if (flg)
		{
			if (!stage.ojamaFlg)
			{
				int size = 0;
				if (size = static_cast<int>(stage.ojamaList_.size()))
				{
					int count = 0;								// 最大何個降らせるか
					bool tmpNam[6]{ false };
					for (auto ojama : stage.ojamaList_)
					{
						ojama->SetSpeed(16, 0);
						ojama->SetFall(true);

						if (size < 6)
						{
							// ランダムの生成
							std::random_device rnd;
							std::mt19937 mt(rnd());
							std::uniform_int_distribution<> posRand(1, 6);
							int random = posRand(mt);
							bool tmpFlg = false;

							while (!tmpFlg)
							{
								if (!tmpNam[random - 1])
								{
									tmpNam[random - 1] = true;
									tmpFlg = true;
								}
								else
								{
									random = posRand(mt);
								}
							}

							Vector2 pos{ random * ojama->size() - ojama->rad().x,ojama->pos().y };
							ojama->pos(pos);
						}

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
			stage.ResetRensa();							// 連鎖のリセット
			stage.InstancePuyo();
			stage.CheckMove(stage.puyoVec_[1]);
			stage.CheckMove(stage.puyoVec_[0]);
			stage.stgMode_ = StgMode::Drop;
		}
	}
private:

};