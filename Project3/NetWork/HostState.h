#pragma once
#include "NetWorkState.h"
class HostState :
	public NetWorkState
{
public:
	HostState();
	~HostState();

	NetWorkMode GetMode(void) override{ return NetWorkMode::HOST; }	// モードの取得

	bool CheckPreparation(void);
	bool CheckConnect(void);
	bool GetReceiveData(Vector2& pos)override;
private:

};

