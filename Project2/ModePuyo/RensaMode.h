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
			stage.stgMode_ = StgMode::Delete;
		}
		else
		{

			if (stage.data_[1][4])
			{
				TRACE("プレイヤー%d", stage.id_ + 1);
				TRACE("GAME OVER\n");
				stage.alive_ = false;
				return;
			}

			// 最大連鎖の更新と生成するお邪魔プヨの計算
			if (stage.rensa_ > stage.maxRensa_)						// 連鎖の最大更新
			{
				stage.maxRensa_ = stage.rensa_;
			}

			stage.ojamaCnt_ = (stage.maxRensa_  / 2) * (stage.rensa_ ) * (stage.eraseCnt_ / 4);
			//stage.ojamaCnt_ = (stage.rensa_) * 30;
			stage.rensa_ = 0;
			stage.eraseCnt_ = 0;

			// 相殺する前にまずお邪魔リストを持っているか確認する
			if (stage.ojamaList_.size())
			{	
				// 相殺する
				// 計算した結果お邪魔リストより数が多かったらその分引いて送る
				if (stage.ojamaCnt_ > static_cast<int>(stage.ojamaList_.size()))
				{
					stage.ojamaList_.clear();
					stage.ojamaCnt_ -= static_cast<int>(stage.ojamaList_.size());
				}
				else
				{
					stage.ojamaList_.erase(stage.ojamaList_.begin(), std::next(stage.ojamaList_.begin(), stage.ojamaCnt_));
					stage.ojamaCnt_ = 0;
				}
			}

			// むにょんしていいか調べるためのラムダ
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
			// どっちの方向にむにょんするか調べる
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