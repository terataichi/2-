#pragma once
#include <vector>
#include "BaseScene.h"
#include "../common/Button.h"

class MenuScene :
	public BaseScene
{
public:
	MenuScene(uniqueBase& child, bool move_);
	~MenuScene();

	uniqueBase Update(uniqueBase own) override;
	void Draw(void)override;
private:
	void Init(void);

	bool move_;											// メニューでアップデートを動かすか動かさないか
	uniqueBase childScene_;								// メニューを開いたときに前のシーンを保存しておく
	std::vector<std::unique_ptr<Button>> button_;		// ぼたん管理用
};

