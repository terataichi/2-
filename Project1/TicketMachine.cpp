#include "TicketMachine.h"
#include "_debug/_DebugConOut.h"
#include <DxLib.h>
#include <algorithm>
#include "MySelf.h"

bool TicketMachine::InitDraw(void)
{
	draw.try_emplace(PayType::MAX, 
	[&](){
			DrawGraph(0, 0, images["money"], true);
			DrawString(
				0,
				comment_offsetY + GetFontSize() / 2,
				"左の枠内の現金化ICカードを選択しクリックして入力してください。\n入金が完了したら決済ボタンを押してください。",
				0xffffff
			);
	});

	draw.try_emplace(PayType::CASH,
	[&](){
			int moneyLine = 0;
			int totalMoney = 0;

			DrawGraph(0, 0, images["act_cash"], true);

			if (paySuccess)
			{
				DrawString(
					0,
					comment_offsetY + GetFontSize() / 2,
					"決済完了\nお釣りを受け取る際は受け取りボタンを押してください",
					0xffffff
				);

				DrawString(draw_offsetX, draw_offsetY, "投入金額", 0xffffff);
				DrawString(draw_offsetX, draw_offsetY, "　　　　　　枚数", 0xffffff);

				for (auto moneyData : cashData)
				{
					DrawFormatString(
						draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(),
						0xffffff, "%d円", moneyData.first
					);
					DrawFormatString(
						draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(),
						0xffffff, "　　　　　%d枚", moneyData.second
					);

					moneyLine++;
					totalMoney += (moneyData.first * moneyData.second);
				}

				DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(),
					0xffffff, "合計投入額　%d円", totalMoney
				);

				DrawString(draw_offsetX * 2, draw_offsetY, "   釣銭", 0xffffff);
				DrawString(draw_offsetX * 2, draw_offsetY, "　　　　　  枚数", 0xffffff);

				int changeLine = 0;
				for (auto data : cashDataChange)
				{
					DrawFormatString(
						draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(),
						0xffffff, "%5d円", data.first
					);
					DrawFormatString(
						draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(),
						0xffffff, "　　　　　%9d枚", data.second
					);
					changeLine++;
				}
			}
			else
			{

				DrawString(
					0,
					comment_offsetY + GetFontSize() / 2,
					"左の枠内の現金を選択しクリックして入力してください。\n入金が完了したら決済ボタンを押してください。",
					0xffffff
				);

				DrawString(draw_offsetX, draw_offsetY, "投入金額", 0xffffff);
				DrawString(draw_offsetX, draw_offsetY, "　　　　　　枚数", 0xffffff);

				for (auto moneyData : cashData)
				{
					DrawFormatString(
						draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(),
						0xffffff, "%d円", moneyData.first
					);
					DrawFormatString(
						draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(),
						0xffffff, "　　　　　%d枚", moneyData.second
					);

					moneyLine++;
					totalMoney += (moneyData.first * moneyData.second);
				}

				DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(),
					0xffffff, "合計投入額　%d円", totalMoney
				);

				// お金がない
				if (totalMoney < price_cash)
				{
					DrawString(draw_offsetX, (draw_offsetY + GetFontSize() * 3) + moneyLine * GetFontSize(),
						"金額が足りません", 0xff0000, true);
				}
			}
	});

	draw.try_emplace(PayType::CARD,
	[&](){
			DrawGraph(0, 0, images["act_card"], true);
			if (paySuccess)
			{
				DrawString(
					0,
					comment_offsetY + GetFontSize() / 2,
					"決済完了\nICカードを出す際は受け取りボタンを押してください",
					0xffffff
				);

				DrawString(draw_offsetX + GetFontSize(), draw_offsetY, "電子マネー", 0xffffff);
				DrawFormatString(
					draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + GetFontSize(),
					0xffffff, "残高　%d円", cardData.first
				);
				DrawFormatString(
					draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + GetFontSize() * 2,
					0xffffff, "引去額　%d円", cardData.second
				);

			}
			else
			{

				DrawString(
					0,
					comment_offsetY + GetFontSize() / 2,
					"左の枠内のICカードを選択しクリックして入力してください。\n入金が完了したら決済ボタンを押してください。",
					0xffffff
				);
				DrawString(draw_offsetX + GetFontSize(), draw_offsetY, "電子マネー", 0xffffff);
				DrawFormatString(
					draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + GetFontSize(),
					0xffffff, "残高　%d円", cardData.first
				);
				// お金がない
				if (cardData.first < price_cash)
				{
					DrawString(draw_offsetX, (draw_offsetY + GetFontSize() * 3), "残高が足りません", 0xff0000, true);
				}
			}
	});

	return false;
}

bool TicketMachine::InitPay(void)
{
	// 決済処理登録
	pay.try_emplace(PayType::MAX, &TicketMachine::PayMax);
	pay.try_emplace(PayType::CASH,&TicketMachine::PayCash);
	pay.try_emplace(PayType::CARD, &TicketMachine::PayCard);
	return false;
}

bool TicketMachine::PayCash(void)
{
	int totalCash = 0;
	auto tmpCashData = cashData;

	for (auto moneyData : tmpCashData)
	{
		totalCash += (moneyData.first * moneyData.second);
	}

	if (totalCash < price_cash)
	{
		// 金額不足
		return false;
	}

	int credit = price_cash;	// 計算用

	for (auto& data : tmpCashData)
	{
		while (data.second)
		{
			credit -= data.first;	
			data.second--;
			TRACE("使ったお金 : %d\n", data.first);
			if (credit <= 0)
			{
				break;
			}
		}
		if (credit <= 0)
		{
			break;
		}
	}
	if (credit <= 0)
	{
		// 支払いが終わったのでお釣りの処理
		cashDataChange = tmpCashData;
		int change = -credit;

		std::for_each(moneyType.crbegin(), moneyType.crend(),
			[&](int type)
			{
				while (change >= type)
				{
					cashDataChange.try_emplace(type, 0);
					cashDataChange[type]++;
					change -= type;
				}
			});

		if (change == 0)
		{
			paySuccess = true;
			return true;
		}
		if (change < 0)
		{
			TRACE("お釣り異常\n");
			return false;
		}
	}

	paySuccess = false;
	return false;
}

bool TicketMachine::PayCard(void)
{
	if (cardData.first > price_card)
	{
		cardData.second = price_card;
		paySuccess = true;
		return true;
	}
	return false;
}

bool TicketMachine::PayMax(void)
{
	return false;
}

void TicketMachine::Clear(void)
{
	btnKey = "btn";
	paySuccess = false;
	payType = PayType::MAX;
	cashData.clear();
	cashDataChange.clear();
	cardData = { 0,0 };
}

void TicketMachine::DrawBtn(void)
{

	SetFontSize(font_size * 2);

	std::string btnName = (paySuccess == false ? "決済" : "受け取り");

	DrawGraph(btnpos.x, btnpos.y, images[btnKey], true);

	DrawString(
		(screen_sizeX - money_sizeX * 2) - (pay_btn_sizeX / 2) - (font_size / 2 * static_cast<int>(btnName.size())), 
		money_sizeY * (moneyType.size()) + font_size / 2,
		btnName.c_str(),
		0xfff
	);
}

bool TicketMachine::Init(sharedMouse mouse)
{
	TicketMachine::mouse = mouse;
	if (!TicketMachine::mouse)
	{
		return false;
	}

	// UI配置されているお金を追加
	if (moneyType.size() == 0)
	{
		moneyType.emplace_back(10);
		moneyType.emplace_back(50);
		moneyType.emplace_back(100);
		moneyType.emplace_back(500);
		moneyType.emplace_back(1000);
		moneyType.emplace_back(5000);
		moneyType.emplace_back(10000);
	}

	images.try_emplace("btn", LoadGraph("image/btn.png"));
	images.try_emplace("btn_push", LoadGraph("image/btn_push.png"));
	// 画像読み込み
	images.try_emplace("money", LoadGraph("image/money.png"));
	images.try_emplace("act_cash", LoadGraph("image/active_cash.png"));
	images.try_emplace("act_card", LoadGraph("image/active_card.png"));
	btnpos = { (screen_sizeX - money_sizeX * 2) - pay_btn_sizeX ,money_sizeY * static_cast<int>(moneyType.size()) };

	InitDraw();
	return true;
}

void TicketMachine::Run(void)
{
	Vector2 pos = mouse->GetPos();

	auto checkBtn = [&]() {	
		return (pos.x >= btnpos.x) && (pos.x < btnpos.x + pay_btn_sizeX) &&
			(pos.y >= btnpos.y) && (pos.y < btnpos.y + pay_btn_sizeY);
	};

	if (mouse->GetClicking() && checkBtn())
	{
		btnKey = "btn_push";
	}
	else
	{
		btnKey = "btn";
	}

	if (mouse->GetClickTrg() && checkBtn())
	{
		if (paySuccess)
		{
			// 決済処理
			switch (payType)
			{
			case PayType::CASH:
				if (lpMySelf.MergeCash(cashDataChange))
				{
					Clear();
				}
				break;
			case PayType::CARD:
				if (lpCardServer.Payment(price_card))
				{
					Clear();
				}
				break;
			case PayType::MAX:
				break;
			default:
				TRACE("エラー：未知の支払い方法\n");
				payType = PayType::MAX;
				break;
			}
			// 決済完了

		}
		else
		{
			// 例外処理
			if (pay.find(payType) != pay.end())
			{
				if ((this->*pay[payType])())
				{
					TRACE("決済成功\n");
				}
			}
			else
			{
				TRACE("エラー：未知の支払い方法\n");
				payType = PayType::MAX;
			}
		}
	}
}

bool TicketMachine::InsertCash(int cash)
{
	//insert = InsertCard(payType,)

	// 見てなかったら要素を追加する
	cashData.try_emplace(cash, 0);
	cashData[cash]++;

	return true;
}

bool TicketMachine::InsertCard(void)
{
	if (payType == PayType::MAX)
	{
		payType = PayType::CARD;
	}
	else
	{
		// カードおよび現金が未投入の場合のみ受け付けるので、それ以外は処理しない
		return false;
	}
	// 情報を入れる
	cardData = lpCardServer.GetCardState();
	return true;
}

void TicketMachine::Draw(void)
{
	SetFontSize(font_size);

	// C++20からcantains
	//if (draw.contains(payType))
	//{
	//	draw[payType]();
	//}

	//if (draw.count(payType))
	//{
	//	draw[payType]();
	//}

	if (draw.find(payType) != draw.end())
	{
		draw[payType]();
	}
	DrawBtn();
}

VecInt& TicketMachine::GetMoneyType(void)
{
	return moneyType;
}

TicketMachine::TicketMachine() :
	comment_offsetY(450), draw_offsetX(200), draw_offsetY(70), price_cash(130), price_card(124),pay_btn_sizeX(200),pay_btn_sizeY(50)
	, screen_sizeX(800), screen_sizeY(600), money_sizeX(100), money_sizeY(50), font_size(18)
{
	InitPay();
	Clear();
	TRACE("TicketMachineを生成\n");
}

TicketMachine::~TicketMachine()
{
	TRACE("TicketMachineを破棄\n");
}
