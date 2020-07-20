#pragma once
#include <memory>
#include "Stage.h"

class PlayUnit
{
public:
	PlayUnit(Stage& stage);
	~PlayUnit();

	void UpDate(void);

private:
	Stage stage_;
};

