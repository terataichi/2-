#pragma once
#include "Vector2.h"


class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	bool GetClickTrg(void);					// クリックした場合、true
	bool GetClicking(void);					// クリック中の場合、true
	const Vector2& GetPos(void);			// ｸﾘｯｸ位置の取得
	void Update(void);
private:
	Vector2 pos;
	char mouseData;		// 現在のクリック情報
	char mouseDataOld;	// 1ﾌﾚｰﾑ前のマウスクリック情報
};

