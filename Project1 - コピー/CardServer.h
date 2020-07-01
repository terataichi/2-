#pragma once
#include <memory>

#define lpCardServer CardServer::GetInstance()

using PairInt = std::pair<int, int>;

class CardServer
{
public:
	static CardServer& GetInstance()
	{
		static CardServer s_Instance;
		return s_Instance;
	}

	PairInt GetCardState(void);				// カードの状態取得
	bool Payment(int price);
private:

	CardServer();
	~CardServer();

	PairInt cardData;						// 残高：引去額
};

