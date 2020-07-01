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

	PairInt GetCardState(void);				// �J�[�h�̏�Ԏ擾
	bool Payment(int price);
private:

	CardServer();
	~CardServer();

	PairInt cardData;						// �c���F�����z
};

