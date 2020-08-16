#pragma once
#include "../Stage.h"

struct FallMode
{
	void operator()(Stage& stage)
	{
		bool nextFlg = true;
		for (auto puyo : stage.puyoVec_)
		{
			// �܂������Ă������`�F�b�N��������
			puyo->SetOldDirFlg();
			nextFlg &= stage.CheckMove(puyo);
		}
		std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
			{
				if (uniPuyo->id() != PuyoID::Ojama)
				{
					// �Ղ�񂳂���
					if (uniPuyo->GetDirFlg().bit.down && !uniPuyo->GetOldDirFlg().bit.down)
					{
						Vector2 grid{ 0,0 };
						grid = uniPuyo->GetGrid(stage.blockSize_);

						auto max = (STAGE_Y - 1) - grid.y;
						if (max > 3)
						{
							max = 3;
						}

						for (int j = 0; j < max; j++)
						{
							if (stage.data_[grid.y + j][grid.x]->id() != PuyoID::Ojama)
							{
								stage.data_[grid.y + j][grid.x]->SetPuyon();
								stage.data_[grid.y + j][grid.x]->SetCnt((max - j));
								if (j >= 3)
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
					}
				}
			});

		bool puyon = true;		// �Ղ��ɍs���Ă������ǂ���
		int count = 0;

		// �S�����񂵂����Ă��邩
		bool fall = true;
		std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
			{
				if (uniPuyo->id() == PuyoID::Ojama)
				{
					fall &= uniPuyo->seiretu();
				}
			});

		std::for_each(stage.ojamaList_.rbegin(), stage.ojamaList_.rend(), [&](SharePuyo& ojama)
			{
				if (!ojama->CheckFall())
				{
					ojama->UpDate(count);
					count++;
				}
			});

		// �����鏈��
		std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
			{
				if (fall)
				{
					uniPuyo->SetDrop();
				}

				// false��������܂������Ă邩��A���ɂ����Ȃ� 
				if (!uniPuyo->UpDate(0))
				{
					Vector2 grid = uniPuyo->GetGrid(stage.blockSize_);
					stage.data_[grid.y][grid.x].reset();
					puyon = false;
				}
			});

		if (puyon)
		{
			stage.stgMode_ = StgMode::Puyon;
		}
	}
};