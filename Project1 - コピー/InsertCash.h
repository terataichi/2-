#pragma once
#include "TicketMachine.h"

struct InsertCash
{
	void operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		// Œ©‚Ä‚È‚©‚Á‚½‚ç—v‘f‚ð’Ç‰Á‚·‚é
		cashData.try_emplace(cash, 0);
		cashData[cash]++;
		payType = PayType::MAX;
	}
};