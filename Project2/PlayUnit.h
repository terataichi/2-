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
	const bool target(void);
private:
	void Init(void);
	void InitFunc(void);
	std::map<INPUT_ID, std::function<bool(INPUT_ID id)>> keyFunc_;			// 入力によって処理が違うので
	Stage& stage_;
	bool target_;
};

