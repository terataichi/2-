#pragma once
#include <map>
#include "InsertMax.h"
#include "MySelf.h"
#include "InsertCard.h"
#include "InsertCash.h"
#include "TicketMachine.h"

struct InsertMax
{
	void operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		if (payType == PayType::CARD)
		{
			lpMySelf.SetIns(InsertCard());
		}
		else if (payType == PayType::CASH)
		{
			lpMySelf.SetIns(InsertCash());
		}
	}
};