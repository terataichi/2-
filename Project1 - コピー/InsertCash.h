#pragma once
#include "TicketMachine.h"

struct InsertCash
{
	void operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		// ���ĂȂ�������v�f��ǉ�����
		cashData.try_emplace(cash, 0);
		cashData[cash]++;
		payType = PayType::MAX;
	}
};