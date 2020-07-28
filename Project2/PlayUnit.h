#pragma once
#include "Stage.h"

class PlayUnit
{
public:
	PlayUnit(Stage& stage);
	~PlayUnit();

	void UpDate(void);
	void Init(void);
private:
	Stage& stage_;
	bool target;
};

