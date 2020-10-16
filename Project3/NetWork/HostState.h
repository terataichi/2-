#pragma once
#include "NetWorkState.h"
class HostState :
	public NetWorkState
{
public:
	HostState();
	~HostState();

	NetWorkMode GetMode(void) override{ return NetWorkMode::HOST; }	// ƒ‚[ƒh‚Ìæ“¾

	//ActiveState CheckPreparation(void);
	bool CheckConnect(void) override;
private:

};

