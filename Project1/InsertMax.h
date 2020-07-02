#pragma once
#include <map>
#include "InsertMax.h"
#include "MySelf.h"
#include "InsertCard.h"
#include "InsertCash.h"
#include "TicketMachine.h"

struct InsertMax
{
	bool operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
	{
		if (payType == PayType::CARD)
		{
			lpMySelf.SetIns(InsertCard());
			return true;
		}
		if (payType == PayType::CASH)
		{
			lpMySelf.SetIns(InsertCash());
			return true;
		}
		return false;
	}
};