#pragma once
#include "TicketMachine.h"
#include "CardServer.h"

struct InsertCard
{
	bool operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		if (payType != PayType::CARD)
		{
			return false;
		}
		// î•ñ‚ğ“ü‚ê‚é
		cardData = lpCardServer.GetCardState();
		return true;
	}
};