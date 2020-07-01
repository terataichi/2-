#pragma once
#include <map>
#include "TicketMachine.h"
#include "MySelf.h"

struct InsertMax
{
	void operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash);
//private:
//	static std::map<PayType, InsFnc> insMap;
};