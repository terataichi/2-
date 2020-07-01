#pragma once
#include "TicketMachine.h"
#include "CardServer.h"

struct InsertCard
{
	void operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		// î•ñ‚ğ“ü‚ê‚é
		cardData = lpCardServer.GetCardState();
	}
};