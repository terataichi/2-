#pragma once

enum class INPUT_ID
{
	BUTTON_LEFT,
	BUTTON_UP,
	BUTTON_RIGHT,
	BUTTON_DOWN,
	BUTTON_ATTACK,
	BUTTON_ROTA_R,
	BUTTON_MENU,			// ���j���[�ڂ���
	BUTTON_MOVEON,			// ���ɐi��
	BUTTON_RETURN,			// �߂�
	MAX,
};

INPUT_ID begin(INPUT_ID);
INPUT_ID end(INPUT_ID);
INPUT_ID operator++(INPUT_ID& key);
INPUT_ID operator*(INPUT_ID key);