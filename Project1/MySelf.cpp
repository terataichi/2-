#include "MySelf.h"
#include "_debug/_DebugConOut.h"
#include <DxLib.h>
#include "MouseCtl.h"
#include "TicketMachine.h"
#include "InsertMax.h"
#include "InsertCard.h"
#include "InsertCash.h"

MySelf* MySelf::s_Instance = nullptr;

bool MySelf::Run()
{
	if (!SysInit())
	{
		return false;
	}
	if (!MyInit())
	{
		return false;
	}
	if (!lpTicketMachine.Init(mouse))
	{
		return false;
	}

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		mouse->Update();

		if (mouse->GetClickTrg() && !lpTicketMachine.GetPaySuccess())
		{
			//TRACE("クリックされました。\n");
			Vector2 pos = mouse->GetPos();
			const VecInt& moneyType = lpTicketMachine.GetMoneyType();
			int type = 0;
			if (pos.x < money_sizeX)
			{
				if (pos.y < money_sizeY * static_cast<int>(moneyType.size()))
				{
					//TRACE("範囲内でした\n");
					type = moneyType[pos.y / money_sizeY];
					if (cash[type] > 0)
					{
						// カードの時は処理できないのではじく
						if (lpTicketMachine.GetPayType() != PayType::CARD)
						{
							lpTicketMachine.SetPayType(PayType::CASH);
							insMap[lpTicketMachine.GetPayType()](lpTicketMachine.GetCashData(), lpTicketMachine.GetCardData(), type);
							cash[type]--;
						}
					}
				}
				else
				{
					// 現金の範囲+１の位置がちょうど電子マネー
					if (pos.y < money_sizeY * static_cast<int>(moneyType.size() + 1))
					{
						if (lpTicketMachine.GetPayType() != PayType::CASH)
						{
							lpTicketMachine.SetPayType(PayType::CARD);
							insMap[lpTicketMachine.GetPayType()](lpTicketMachine.GetCashData(), lpTicketMachine.GetCardData(), type);
						}
					}
				}
			}
		}
		lpTicketMachine.Run();
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		Draw();
		ScreenFlip();
	}

	return true;
}

void MySelf::Draw(void)
{
	int moneyLine = 0;
	SetFontSize(font_size);
	for(auto cashData : cash)
	{
		DrawFormatString(money_sizeX + 25, money_sizeY * moneyLine + money_sizeY / 3, 0xffffff, "%2d 枚", cashData.second);
		moneyLine++;
	}

	const VecInt& moneyType = lpTicketMachine.GetMoneyType();
	// 仕切り線
	DrawLine(0, money_sizeY * (static_cast<int>(moneyType.size()) + 2),
		screen_sizeX, money_sizeY * (static_cast<int>(moneyType.size()) + 2),
		0xffffff, true);

	DrawLine(money_sizeX * 2,
		0,
		money_sizeX * 2,
		money_sizeY * (static_cast<int>(moneyType.size()) + 2),
		0xffffff,
		true);



	lpTicketMachine.Draw();
}

bool MySelf::MergeCash(MapInt& changecash)
{
	for (auto data : changecash)
	{
		cash[data.first] += data.second;
	}
	return true;
}

MySelf::MySelf() :screen_sizeX(800), screen_sizeY(600),money_sizeX(100),money_sizeY(50),font_size(18)
{
	mouse = nullptr;
	TRACE("MySelfを生成\n");
}

MySelf::~MySelf()
{
	TRACE("MySelfを破棄\n");
}

bool MySelf::SysInit()
{
	SetGraphMode(screen_sizeX, screen_sizeY, 16);
	ChangeWindowMode(true);
	SetWindowText("a");


	if (DxLib_Init() == -1)
	{
		return false;
	}
	SetFontSize(font_size);
	return true;
}

bool MySelf::MyInit()
{
	// 宣言と中身を一緒にする
	// std::unique_ptr<MouseCtl> mouse(new MouseCtl());
	//mouse.reset(new MouseCtl());

	// 所有権を渡す
	//std::unique_ptr<MouseCtl> tmp;
	//tmp = std::move(mouse);

	mouse = std::make_shared<MouseCtl>();

	if (!mouse)
	{
		return false;
	}

	// 保持する現金を追加
	cash.try_emplace(static_cast<int>(10), 15);
	cash.try_emplace(static_cast<int>(50), 3);
	cash.try_emplace(static_cast<int>(100), 2);
	cash.try_emplace(static_cast<int>(500), 1);
	cash.try_emplace(static_cast<int>(1000), 1);
	cash.try_emplace(static_cast<int>(5000), 1);
	cash.try_emplace(static_cast<int>(10000), 1);
	//cash.try_emplace(static_cast<int>(Cash::Card), 15);

	insMap.try_emplace(PayType::MAX, InsertMax());
	insMap.try_emplace(PayType::CASH, InsertCash());
	insMap.try_emplace(PayType::CARD, InsertCard());
	//TRACE("money.png:%d", images["money"]);
	return true;
}
