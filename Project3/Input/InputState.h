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

	const TrgData& GetTrgData(void);			// ƒf[ƒ^‚ÌƒQƒbƒgŠÖ”
	const bool GetTrgOnePull(INPUT_ID);			// —£‚³‚ê‚½‚©uŠÔ‚©‚Ç‚¤‚©‚ğæ“¾(!now && old)
	const bool GetTrgOnePush(INPUT_ID);			// ‰Ÿ‚³‚ê‚½uŠÔ‚©‚Ç‚¤‚©‚ğæ“¾(now && !old)
	const bool GetTrgPull(INPUT_ID);			// ‰Ÿ‚³‚ê‚½uŠÔ‚©‚Ç‚¤‚©‚ğæ“¾(now && !old)
	const bool GetTrgPush(INPUT_ID);			// ‰Ÿ‚³‚ê‚½uŠÔ‚©‚Ç‚¤‚©‚ğæ“¾(now && !old)

	virtual CntType GetCntType(void) = 0;		// ©•ª‚ª‚¢‚Ü‰½‚ğg‚Á‚Ä‚¢‚é‚Ì‚©‚ğ•Ô‚·
	virtual bool SetUp(int no) = 0;

private:
	virtual void Update(void) = 0;

protected:

	TrgData _state;								// ‚»‚ê‚¼‚ê‚Ìkey‚Ìnew ‚Æ old‚Ìî•ñŠi”[
};

