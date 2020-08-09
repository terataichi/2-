#pragma once
#include "../Stage.h"
#include "../_debug/_DebugConOut.h"

struct RensaMode
{
	void operator()(Stage& stage)
	{
		bool deleteFlg = false;

		//Vector2 grid = puyoVec_[0]->GetGrid(blockSize_);
		//data_[grid.y][grid.x] = puyoVec_[0];

		for (auto&& pvec : stage.puyoVec_)
		{
			deleteFlg |= stage.SetErase(pvec, pvec->GetGrid(stage.blockSize_));
		}

		if (deleteFlg)
		{
			// �����ŏ�����O�ɃA�j���[�V�������Z�b�g����
			//for (auto&& pvec : stage.puyoVec_)
			//{
			//	if (!pvec->alive())
			//	{
			//		//pvec->PlayDeathAnim();
			//	}
			//}
			stage.stgMode_ = StgMode::Delete;
		}
		else
		{

			if (stage.data_[1][4])
			{
				TRACE("�v���C���[%d", stage.id_ + 1);
				TRACE("GAME OVER\n");

				stage.stgMode_ = StgMode::GameOver;
				return;
			}

			// �ő�A���̍X�V�Ɛ������邨�ז��v���̌v�Z
			if (stage.rensa_ > stage.maxRensa_)						// �A���̍ő�X�V
			{
				stage.maxRensa_ = stage.rensa_;
			}

			stage.ojamaCnt_ = (stage.maxRensa_ / 2) * (stage.rensa_) * (stage.eraseCnt_ / 8);
			stage.ojamaCnt_ = (stage.rensa_) * 30;
			stage.eraseCnt_ = 0;

			// �ނɂ�񂵂Ă��������ׂ邽�߂̃����_
			auto punyonBit = [&](PuyoID id, Vector2 vec)
			{
				if (stage.data_[vec.y][vec.x])
				{
					if (stage.data_[vec.y][vec.x]->id() != PuyoID::Ojama)
					{
						return stage.data_[vec.y][vec.x]->id() == id;
					}
				}
				return false;
			};

			Vector2 grid{ 0,0 };
			// �ǂ����̕����ɂނɂ�񂷂邩���ׂ�
			for (auto vec : stage.puyoVec_)
			{
				DirUnion bit = { 0,0,0,0 };		
				grid = vec->GetGrid(stage.blockSize_);
				bit.bit.left = punyonBit(vec->id(), Vector2(grid.x - 1, grid.y));
				bit.bit.right = punyonBit(vec->id(), Vector2(grid.x + 1, grid.y));
				bit.bit.up = punyonBit(vec->id(), Vector2(grid.x, grid.y - 1));
				bit.bit.down = punyonBit(vec->id(), Vector2(grid.x, grid.y + 1));
				vec->SetMunyon(20);
				vec->SetMnyonFlg(bit);
			}

			stage.stgMode_ = StgMode::Munyon;
		}
	}
};