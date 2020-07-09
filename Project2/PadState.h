#pragma once
#include "InputState.h"
class PadState :
	public InputState
{
public:
	PadState(int id);
	~PadState();

	void UpDate(void) override;
private:
	std::vector<int> _keyCon;					// ÉLÅ[ÇÃìoò^
	std::vector<int> _keyConDef;				//
	int _padID;
};

