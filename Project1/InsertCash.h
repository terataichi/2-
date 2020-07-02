#pragma once
#include "TicketMachine.h"

struct InsertCash
{
	bool operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		if (payType != PayType::CASH)
		{
			return false;
		}
		// 見てなかったら要素を追加する
		cashData.try_emplace(cash, 0);
		cashData[cash]++;
		return true;
	}
};