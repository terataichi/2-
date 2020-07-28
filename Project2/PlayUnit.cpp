#include "PlayUnit.h"
#include "_debug/_DebugConOut.h"

PlayUnit::PlayUnit(Stage& stage):stage_(stage)
{
	target = 0;
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

	if (stage_.input_->GetTrgData().at(INPUT_ID::BUTTON_DOWN)[static_cast<int>(Trg::Now)])
	{
		stage_.puyoVec_[target]->SoftDrop();
		stage_.puyoVec_[target^1]->SoftDrop();
	}

	auto rota = [&](bool flg, Vector2&& pos)
	{
		if (flg)
		{
			stage_.puyoVec_[target^1]->pos(pos);
		}
	};

	for (auto data : stage_.input_->GetTrgData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			stage_.puyoVec_[target]->Move(data.first);
			stage_.puyoVec_[target^1]->Move(data.first);

			if ((data.first == INPUT_ID::BUTTON_ROTA_L) || (data.first == INPUT_ID::BUTTON_ROTA_R))
			{
				int rotate = 1;
				if ((data.first == INPUT_ID::BUTTON_ROTA_R))
				{
					rotate = (-1);
				}

				auto pos1 = stage_.puyoVec_[target]->pos();
				auto pos2 = stage_.puyoVec_[target^1]->pos();
				auto size = stage_.blockSize_ ;
				rota(pos1.x > pos2.x, pos2 + Vector2(size, -size * rotate));
				rota(pos1.x < pos2.x, pos2 + Vector2(-size, size * rotate));
				rota(pos1.y > pos2.y, pos2 + Vector2(size * rotate, size));
				rota(pos1.y < pos2.y, pos2 + Vector2(-size * rotate, -size));

				if (stage_.puyoVec_[target]->pos().y < stage_.puyoVec_[target ^ 1]->pos().y)
				{
					std::swap(stage_.puyoVec_[0], stage_.puyoVec_[1]);
					target = target ^ 1;
					TRACE("%d", target);
				}
			}
		}
	}
}

void PlayUnit::Init(void)
{
}
