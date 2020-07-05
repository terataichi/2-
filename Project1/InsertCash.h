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
		// Œ©‚Ä‚È‚©‚Á‚½‚ç—v‘f‚ð’Ç‰Á‚·‚é
		cashData.try_emplace(cash, 0);
		cashData[cash]++;
		return true;
	}
};