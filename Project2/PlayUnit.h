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
	void InitFunc(void);
	std::map<INPUT_ID, std::function<bool(INPUT_ID id)>> keyFunc_;			// “ü—Í‚É‚æ‚Á‚Äˆ—‚ªˆá‚¤‚Ì‚Å
	Stage& stage_;
	bool target;
};

