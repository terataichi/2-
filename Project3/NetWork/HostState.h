#pragma once
#include "NetWorkState.h"
#include "../Scene/SceneMng.h"

class HostState :
	public NetWorkState
{
public:
	HostState();
	~HostState();

	NetWorkMode GetMode(void) override{ return NetWorkMode::HOST; }	// ���[�h�̎擾

	//ActiveState CheckPreparation(void);
	bool CheckConnect(void) override;
private:

	chronoTime startTime_;
	bool startFlg_ = false;
	int connectCnt_;
};

