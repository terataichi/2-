#pragma once
#include "_debug/_DebugConOut.h"

struct InsertMax
{
	bool operator()(MapInt& cashData, PairInt& cardData, int cash)
	{
		TRACE("設定されていません");
		return false;
	}
};