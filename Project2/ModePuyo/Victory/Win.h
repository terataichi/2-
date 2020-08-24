#pragma once
#include "../../Stage.h"

struct Win
{
	bool operator()(Stage& stage)
	{
		// ���̏ꏊ�܂ŃX�e�[�W�𗎂Ƃ�
		int maxY = lpSceneMng.screenSize_.y + stage.size_.y;
		if (winY < maxY)
		{
			winY += speed;
		}

		//TRACE("�v���C���[ %d Win\n",stage.id_ + 1);
		// �J�E���g�_�E����
		if (stage.gameOverCnt_-- < 0)
		{
			// �I��
			return true;
		}
		// ����
		if (stage.gameOverCnt_ % 2 == 0)
		{
			speed++;
		}

		int standard = 0;
		float angle = 0;
		float angleStnd = 36.0f;

		// �������Ƃɓ�����t����
		Vector2 div{ 5,1 };
		for (int j = 0; j < div.x; j++)
		{
			int id = lpImageMng.GetHandle("win", div, { 64,64 })[j];

			standard = static_cast<int>((maxY - winY) + (stage.size_.y / 3) - 50.0f * cosf((count + (j + 1) * 30.0f) * 0.01f));			// �T�C���J�[�u

			angle = (-DX_PI_F / angleStnd) - (DX_PI_F / (angleStnd / 2)) * cosf((count + (j + 1) * 30.0f) * 0.01f);

			lpSceneMng.AddDrawQue({ id, {stage.offSet_.x + 64 + 64 * j, standard}, angle, 0 });
			count++;
		}
		return false;
	}

private:
	int speed = -5;
	int count = 0;
	int winY = 0;
};