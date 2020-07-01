#include "InsertMax.h"
#include "MySelf.h"
#include "InsertCard.h"
//std::map<PayType, InsFnc> InsertMax::insMap{ {PayType::MAX,&} };

void InsertMax::operator()(PayType& payType, MapInt& cashData, PairInt& cardData, int cash)
{
	if (payType == PayType::CARD)
	{
		//lpMySelf.Insert(InsertCard());
		return;
	}
	else if (payType == PayType::CARD)
	{
		// = &InsertCash();
	}
	else
	{
		// = &InsertMax();
	}
}
