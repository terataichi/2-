#pragma once
#include <vector>
#include <array>
#include <map>
#include "INPUT_ID.h"

enum class CntType
{
	Key,
	Pad,
	Mouse
};

enum class Trg
{
	Now,
	Old,
	Max
};

using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;
using TrgData = std::map<INPUT_ID, TrgBool>;

struct InputState
{
	void operator()()
	{
		Update();
	}

	const TrgData& GetTrgData(void);			// �f�[�^�̃Q�b�g�֐�
	const bool GetTrgPull(INPUT_ID);			// �ʃf�[�^�̗����ꂽ���ǂ������擾(!now && old)
	const bool GetTrgPush(INPUT_ID);			// �ʃf�[�^�̗����ꂽ���ǂ������擾(!now && old)
	virtual CntType GetCntType(void) = 0;		// ���������܉����g���Ă���̂���Ԃ�
	virtual bool SetUp(int no) = 0;

private:
	virtual void Update(void) = 0;

protected:

	TrgData _state;								// ���ꂼ���key��new �� old�̏��i�[
};

