#pragma once
#include "InputState.h"
class PadState :
	public InputState
{
public:
	virtual CntType GetCntType(void) override;			// ©•ª‚ª‚¢‚Ü‰½‚ğg‚Á‚Ä‚¢‚é‚Ì‚©‚ğ•Ô‚·
	virtual bool SetUp(int no) override;				// ‰Šú‰»
	virtual void Update(void) override;

	PadState();
	~PadState();
private:
	std::map<INPUT_ID, int> _keyCon;					// ƒL[‚Ì“o˜^
	std::map<INPUT_ID, int> _keyConDef;					//
	int _padID;
};

