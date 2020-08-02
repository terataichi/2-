#pragma once
#include "../Stage.h"
#include "../_debug/_DebugConOut.h"

struct DeleteMode
{
	void operator()(Stage& stage)
	{
		// ������Ƃ��̃A�j���[�V�������Ō�܂ŏI�����Ă���f�[�^�������
		// if(CheckDeathAnim)
		{
			// ����
			auto itr = std::remove_if(stage.puyoVec_.begin(), stage.puyoVec_.end(), [](std::shared_ptr<puyo>& puyo) {return !(puyo->alive()); });
			stage.puyoVec_.erase(itr, stage.puyoVec_.end());

			// �c��̂Ղ�̃X�s�[�h��ς���
			std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
				{
					uniPuyo->SetSpeed(8, 0);
					uniPuyo->UpSpeed();
				});
			stage.AddRensa();										// �����܂ŗ��Ă�Ή��������Ă���̂ŘA���m��
			TRACE("%d�A��\n", stage.rensa());
			stage.stgMode_ = StgMode::FALL;
		}
	}
};