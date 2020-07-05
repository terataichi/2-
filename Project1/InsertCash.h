#pragma once
#include "TicketMachine.h"

struct InsertCash
{
	bool operator()(MapInt& cashData, PairInt& cardData, int cash)
	{
		if (lpTicketMachine.GetPayType() != PayType::CASH)
		{
			return false;
		}
		// ���ĂȂ�������v�f��ǉ�����
		cashData.try_emplace(cash, 0);
		cashData[cash]++;
		return true;
	}
};