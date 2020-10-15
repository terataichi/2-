#pragma once
#include "NetWorkState.h"
class HostState :
	public NetWorkState
{
public:
	HostState();
	~HostState();

	NetWorkMode GetMode(void) override{ return NetWorkMode::HOST; }	// ���[�h�̎擾
	bool CheckConnect(void);
	bool GetReceiveData(Vector2& pos)override;
private:

};

