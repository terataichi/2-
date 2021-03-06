#pragma once
#include "TicketMachine.h"
#include "CardServer.h"

struct InsertCard
{
	bool operator()(MapInt& cashData, PairInt& cardData, int cash)
	{
		if (lpTicketMachine.GetPayType() != PayType::CARD)
		{
			return false;
		}
		// 情報を入れる
		cardData = lpCardServer.GetCardState();
		return true;
	}
};