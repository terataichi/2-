#include "PlayUnit.h"
#include "_debug/_DebugConOut.h"

PlayUnit::PlayUnit(Stage& stage):stage_(stage)
{
	Init();
}

PlayUnit::~PlayUnit()
{
}

void PlayUnit::UpDate(void)
{
	if (stage_.stgMode_ != StgMode::Drop)
	{
		return;
	}

	// どちらかが下についたら動けなくする
	if (stage_.puyoVec_[target]->GetDirFlg().bit.down || stage_.puyoVec_[target ^ 1]->GetDirFlg().bit.down)
	{
		stage_.puyoVec_[target]->SetSpeed(8, 0);
		stage_.puyoVec_[target]->UpSpeed();
		stage_.puyoVec_[target ^ 1]->SetSpeed(8, 0);
		stage_.puyoVec_[target ^ 1]->UpSpeed();
		stage_.stgMode_ = StgMode::FALL;
		return;
	}

	// ソフトドロップ
	if (stage_.input_->GetTrgData().at(INPUT_ID::BUTTON_DOWN)[static_cast<int>(Trg::Now)])
	{
		stage_.puyoVec_[target]->SoftDrop();
		stage_.puyoVec_[target^1]->SoftDrop();
	}

	for (auto data : stage_.input_->GetTrgData())
	{
		// old とnow を見る
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			if (keyFunc_[data.first](data.first))
			{
				//　入力された方向に進もうとする
				stage_.puyoVec_[target]->Move(data.first);
				stage_.puyoVec_[target ^ 1]->Move(data.first);
			}
		}
	}
}

void PlayUnit::SetTarget(void)
{
	target = 0;
}

void PlayUnit::Init(void)
{
	target = 0;
	InitFunc();
}

void PlayUnit::InitFunc(void)
{
	std::function<bool(INPUT_ID id)> LRMove = [&](INPUT_ID id)
	{
		if (id == INPUT_ID::BUTTON_RIGHT)
		{
			if (stage_.puyoVec_[target]->GetDirFlg().bit.right || stage_.puyoVec_[target ^ 1]->GetDirFlg().bit.right)
			{
				return false;
			}
		}
		if (id == INPUT_ID::BUTTON_LEFT)
		{
			if (stage_.puyoVec_[target]->GetDirFlg().bit.left || stage_.puyoVec_[target ^ 1]->GetDirFlg().bit.left)
			{
				return false;
			}
		}

		return true;
	};

	std::function<bool(INPUT_ID id)> RotateMove = [&](INPUT_ID id)
	{
		// データを見て何もなかったら回転していいよ
		auto rota = [&](bool flg, Vector2&& pos, Vector2&& offset, Vector2&& off)
		{
			Vector2 grid = stage_.GetGrid(pos);

			// 中途半端な時に移動するとめり込むので余りが出たらもう一個下を見る
			int set = (pos.y + stage_.blockSize_ / 2) % stage_.blockSize_ != 0;
			
			set = stage_.puyoVec_[target]->pos().y < pos.y;
			 
			if (flg)
			{
				if (!stage_.data_[grid.y + set][grid.x])
				{
					stage_.puyoVec_[target ^ 1]->pos(pos);

					return true;
				}
				else
				{
					if (!stage_.data_[grid.y + set + offset.y][grid.x + offset.x])
					{
						pos -= Vector2{ 64 * off.x ,64 * off.y};
						stage_.puyoVec_[target ^ 1]->pos(pos);
						pos -= Vector2{ 64 * off.x ,64 * off.y };
						stage_.puyoVec_[target]->pos(pos);
					}
					return true;
				}
			}
			return false;
		};

		// 回転
		int rotate = 1;
		if ((id == INPUT_ID::BUTTON_ROTA_R))
		{
			rotate = (-1);
		}

		auto pos1 = stage_.puyoVec_[target]->pos();
		auto pos2 = stage_.puyoVec_[target ^ 1]->pos();
		auto size = stage_.blockSize_;

		int a = 2;
		rota(pos1.x > pos2.x, pos2 + Vector2(size, -size * rotate), Vector2(0, a * rotate), Vector2(0, -1 * rotate));
		rota(pos1.x < pos2.x, pos2 + Vector2(-size, size * rotate), Vector2(0, -a * rotate), Vector2(0, 1 * rotate));
		rota(pos1.y > pos2.y, pos2 + Vector2(size * rotate, size), Vector2(-a * rotate, 0), Vector2(1 * rotate, 0));
		rota(pos1.y < pos2.y, pos2 + Vector2(-size * rotate, -size), Vector2(a * rotate, 0), Vector2(-1* rotate, 0));

		if (stage_.puyoVec_[0]->pos().y > stage_.puyoVec_[1]->pos().y)
		{
			std::swap(stage_.puyoVec_[0], stage_.puyoVec_[1]);
			target = target ^ 1;
			TRACE("%d\n", target);
		}
		return true;
	};

	std::function<bool(INPUT_ID id)> UDMove = [&](INPUT_ID id)
	{
		return true;
	};
	keyFunc_.try_emplace(INPUT_ID::BUTTON_LEFT, LRMove);
	keyFunc_.try_emplace(INPUT_ID::BUTTON_RIGHT, LRMove);
	keyFunc_.try_emplace(INPUT_ID::BUTTON_DOWN, UDMove);
	keyFunc_.try_emplace(INPUT_ID::BUTTON_UP, UDMove);
	keyFunc_.try_emplace(INPUT_ID::BUTTON_ROTA_L, RotateMove);
	keyFunc_.try_emplace(INPUT_ID::BUTTON_ROTA_R, RotateMove);
}
