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

	const TrgData& GetTrgData(void);			// データのゲット関数
	const bool GetTrgPull(INPUT_ID);			// 個別データの離されたかどうかを取得(!now && old)
	const bool GetTrgPush(INPUT_ID);			// 個別データの離されたかどうかを取得(!now && old)
	virtual CntType GetCntType(void) = 0;		// 自分がいま何を使っているのかを返す
	virtual bool SetUp(int no) = 0;

private:
	virtual void Update(void) = 0;

protected:

	TrgData _state;								// それぞれのkeyのnew と oldの情報格納
};

