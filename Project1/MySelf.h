#pragma once
#include <mutex>
#include <map>
#include <vector>
#include <memory>
#include "TicketMachine.h"

#define lpMySelf MySelf::GetInstance()

using InsFnc = std::function<bool(PayType& , MapInt& , PairInt& , int )>;	// 関数オブジェクトの宣言

class MouseCtl;

class MySelf
{
public:
	static MySelf& GetInstance()
	{
		std::once_flag once;
		std::call_once(once, Create);
		return *s_Instance;
	}

	static void Create()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new MySelf();
		}
	}

	static void Destroy()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	bool Run(void);
	void Draw(void);
	bool MergeCash(MapInt& cash);
	void SetIns(InsFnc ins);				// セット
private:
	MySelf(void);
	~MySelf(void);

	bool SysInit();
	bool MyInit();

	const int screen_sizeX;
	const int screen_sizeY;
	const int money_sizeX;					// 画像サイズ
	const int money_sizeY;					// 画像サイズ
	const int font_size;

	InsFnc insert;							// インサートを関数オブジェクトで管理	

	//std::map<PayType, InsFnc> insMap;		// 関数オブジェクト管理用変数
	//std::map<std::string, int> images;	// 画像読み込み用
	MapInt cash;							// 持っているお金

	// unique_ptr → unique_ptr のメンバー
	//   ↑		  → MouseCtlのマンバー
	// mouse
	sharedMouse mouse;

	static MySelf* s_Instance;
};

