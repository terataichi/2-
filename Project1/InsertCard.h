#pragma once
#include "TicketMachine.h"
#include "CardServer.h"

struct InsertCard
{
	void operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		// 情報を入れる
		cardData = lpCardServer.GetCardState();
	}
};