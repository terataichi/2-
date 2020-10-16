#include <DxLib.h>
#include "HostState.h"

HostState::HostState()
{
	//active_ = !PreparationListenNetWork(portNum_);
	bool flg;
	flg = PreparationListenNetWork(portNum_) == 0 ? true : false;

	active_ = ActiveState::Non;

	if (flg)
	{
		// �ڑ��҂��ɓ���
		active_ = ActiveState::Wait;
	}
}

HostState::~HostState()
{
}

//ActiveState HostState::CheckPreparation(void)
//{
//	bool flg;
//	flg = PreparationListenNetWork(portNum_) == 0 ? true : false;
//
//	// �ڑ��҂���Ԃɂ���
//	if (flg) return ActiveState::Wait;
//
//	return ActiveState::Non;
//}

bool HostState::CheckConnect(void)
{
	int handle = GetNewAcceptNetWork();

	if(handle != -1)
	{
		// �ڑ�����Ă�̂ł���ȏ�ڑ�����Ȃ��悤�Ɏ~�߂�
		StopListenNetWork();
		// ��������Ԃɓ���
		netHandle_ = handle;
		active_ = ActiveState::Init;

		TRACE("�ڑ����m�F����܂���");
		return true;
	}
	return false;
}
