#pragma once
#include <map>
#include <functional>
#include "Stage.h"

class PlayUnit
{
public:
	PlayUnit(Stage& stage);
	~PlayUnit();

	void UpDate(void);
	void SetTarget(void);
private:
	void Init(void);
	bool LRMove(INPUT_ID id);
	bool RotateMove(INPUT_ID id);											// ���E��]�p
	std::map<INPUT_ID, std::function<bool(INPUT_ID id)>> keyFunc_;			// ���͂ɂ���ď������Ⴄ�̂�
	Stage& stage_;
	bool target;
};

