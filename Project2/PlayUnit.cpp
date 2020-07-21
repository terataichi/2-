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
	}

	for (auto data : stage_.input_->GetTrgData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			stage_.puyoVec_[0]->Move(data.first);
		}
	}
}
