#include "InsertMax.h"
#include "MySelf.h"
#include "InsertCard.h"
#include "_debug/_DebugConOut.h"

//std::map<PayType, InsFnc> InsertMax::insMap{ {PayType::MAX,&} };

void InsertMax::operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
{
	if (payType == PayType::CARD)
	{
		payType = PayType::CARD;
		return;
	}
	else if (payType == PayType::CASH)
	{
		payType = PayType::CASH;
	}
	//else
	//{
	//	//TRACE("AAAA");
	//}
}
