#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "MouseCtl.h"
#include "CardServer.h"
#include "InsertCard.h"
#include "InsertCash.h"

using MapInt = std::map<int, int>;
using VecInt = std::vector<int>;
using sharedMouse = std::shared_ptr<MouseCtl>;

#define lpTicketMachine TicketMachine::GetInstance()

enum class PayType
{
	CASH,	// 現金
	CARD,	// ICカード
	MAX		// 未設定
};

class TicketMachine
{
public:
	static TicketMachine& GetInstance()
	{
		static TicketMachine s_Instance;
		return s_Instance;
	}	
	void Run(void);																	// メイン処理
	bool InsertCash(int cash);														// お金が入ったかを確認
	bool InsertCard(void);															// キャッシュカード
	void Draw(void);
	VecInt& GetMoneyType(void);
	bool Init(sharedMouse mouse);
private:
	bool InitDraw(void);															// 描画を登録したり
	bool InitPay(void);
	bool PayCash(void);																// 現金払い処理
	bool PayCard(void);																// カード払い処理
	bool PayMax(void);																// 例外処理
	void Clear(void);
	void DrawBtn(void);
	bool paySuccess;																// 支払い完了

	sharedMouse mouse;																// もらうやつ

	std::string btnKey;																// 表示するボタンへのキー
	Vector2 btnpos;																	// ボタンの座標
		
	std::vector<int> moneyType;														// 支払い手法
	PayType payType;																// 支払い方法
	MapInt cashData;																// 現金
	MapInt cashDataChange;															// お釣り用
	PairInt cardData;																// カードデータ

	std::map<std::string, int> images;												// 画像読み込み用

	std::map<PayType, std::function<void(void)>>draw;								// 描画するためのラムダ式を格納するmap
	std::map<PayType, bool (TicketMachine::*)()> pay;								// 決済処理用map
	std::function<bool(void)> f;			

	const int pay_btn_sizeX;														// 支払いボタンの横サイズ
	const int pay_btn_sizeY;
	const int comment_offsetY;
	const int draw_offsetX;															// 文字描画のオフセット
	const int draw_offsetY;
	const int price_card;															// 現金の場合の価格
	const int price_cash;															// 電子マネーの場合の価格

//---------------
	const int screen_sizeX;
	const int screen_sizeY;
	const int money_sizeX;															// 画像サイズ
	const int money_sizeY;															// 画像サイズ
	const int font_size;
//---------------


	TicketMachine();
	~TicketMachine();
	static TicketMachine s_Instance;
};

