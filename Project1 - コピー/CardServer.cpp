#include "CardServer.h"

// Œ»İ‚Ìc‚‚Æˆø‚«Šz‚ğæ“¾‚·‚éŠÖ”
PairInt CardServer::GetCardState(void)
{
	return cardData;
}

bool CardServer::Payment(int price)
{
	if (cardData.first <= price)
	{
		return false;
	}

	cardData.first -= price;
	cardData.second = price;
	return false;
}

CardServer::CardServer()
{
	cardData = { 1000,0 };
}

CardServer::~CardServer()
{
}
