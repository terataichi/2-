#include "PlayUnit.h"

PlayUnit::PlayUnit(Stage& stage):stage_(stage)
{
}

PlayUnit::~PlayUnit()
{
}

void PlayUnit::UpDate(void)
{
	if (stage_.stgMode_ == StgMode::Rensa)
	{
		return;
	}


	if (stage_.input_->GetTrgData().at(INPUT_ID::BUTTON_DOWN)[static_cast<int>(Trg::Now)])
	{
		stage_.puyoVec_[0]->SoftDrop();
		stage_.puyoVec_[1]->SoftDrop();
	}

	auto rota = [&](bool flg, Vector2&& pos)
	{
		if (flg)
		{
			stage_.puyoVec_[1]->pos(pos);
		}
	};

	for (auto data : stage_.input_->GetTrgData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			stage_.puyoVec_[0]->Move(data.first);
			stage_.puyoVec_[1]->Move(data.first);

			if ((data.first == INPUT_ID::BUTTON_ROTA_L) || (data.first == INPUT_ID::BUTTON_ROTA_R))
			{
				int rotate = 1;
				if ((data.first == INPUT_ID::BUTTON_ROTA_R))
				{
					rotate = (-1);
				}

				auto pos1 = stage_.puyoVec_[0]->pos();
				auto pos2 = stage_.puyoVec_[1]->pos();
				auto size = stage_.blockSize_ ;
				rota(pos1.x > pos2.x, pos2 + Vector2(size, -size * rotate));
				rota(pos1.x < pos2.x, pos2 + Vector2(-size, size * rotate));
				rota(pos1.y > pos2.y, pos2 + Vector2(size * rotate, size));
				rota(pos1.y < pos2.y, pos2 + Vector2(-size * rotate, -size));

			}
		}
	}
}
