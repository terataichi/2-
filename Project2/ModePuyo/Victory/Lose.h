#pragma once
#include "../../Stage.h"
#include <cmath>
#include "../../Scene/SceneMng.h"
#include "../../common/ImageMng.h"

struct Lose
{
	bool operator()(Stage& stage)
	{
		//TRACE("�v���C���[ %d Lose\n", stage.id_ + 1);
		stage.angle_ += atanf(0.008f);

		// ���̏ꏊ�܂ŃX�e�[�W�𗎂Ƃ�
		int maxY = lpSceneMng.screenSize_.y + stage.size_.y;
		if (stage.gameOverPos_.y < maxY)
		{
			stage.gameOverPos_.y += speed;
		}

		if (stage.gameOverCnt_ % 2 == 0)
		{
			speed++;
		}

		int standard = 0;
		float angle = 0;
		float angleStnd = 36.0f;

		// �������Ƃɓ�����t����
		Vector2 div{4,1 };
		for (int j = 0; j < div.x; j++)
		{
			int id = lpImageMng.GetHandle("lose2", div, { 48,74 })[j];

			// �T�C���J�[�u				��ʒu�@-+�@���@*�@�J�[�u
			standard = static_cast<int>((maxY - stage.gameOverPos_.y) + (stage.size_.y / 3) - 50.0f * cosf((count + (j + 1) * 30) * 0.01f));
			angle = static_cast<float>((-DX_PI_F / angleStnd) - (DX_PI_F / (angleStnd / 2.0f)) * cosf((count + (j + 1) * 30.0f) * 0.01f));

			lpSceneMng.AddDrawQue({ id, stage.offSet_.x + (stage.blockSize_ + 32) + (stage.blockSize_ * j), standard,1, angle,0 });
			count++;
		}

		// �J�E���g�_�E����
		if (stage.gameOverCnt_-- < 0)
		{
			// �I��
			return true;
		}
		return false;
	}

private:
	int speed = -5;
	int count = 0;
};