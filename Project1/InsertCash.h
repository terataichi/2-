#pragma once
#include "TicketMachine.h"

struct InsertCash
{
	void operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		// 見てなかったら要素を追加する
		cashData.try_emplace(cash, 0);
		cashData[cash]++;
	}
};