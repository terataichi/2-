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
		// 情報を入れる
		cardData = lpCardServer.GetCardState();
		return true;
	}
};