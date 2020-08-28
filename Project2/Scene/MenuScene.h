#pragma once
#include <vector>
#include "BaseScene.h"
#include "../common/Button.h"
#include "../Input/InputState.h"

using uniqueButton = std::unique_ptr<Button>;

// ぼたんに対応したシーンを格納する
using ButtonPairVec = std::vector<std::pair<uniqueButton, uniqueBase>>;

class MenuScene :
	public BaseScene
{
public:
	MenuScene(uniqueBase child, bool update, bool draw, ButtonPairVec buttonVec);
	~MenuScene();

	uniqueBase Update(uniqueBase own) override;
	void Draw(void)override;
private:
	void Init(bool update, bool draw);

	bool childUpdateFlg_;																// メニューで子のアップデートを動かすか動かさないか
	bool childDrawFlg_;																	// メニューで子の描画をするかどうか
	uniqueBase childScene_;																// メニューを開いたときに前のシーンを保存しておく
	ButtonPairVec buttonVec_;															// ぼたん管理用
	std::map<CntType, std::shared_ptr<InputState>> input_;								// キーの入力管理

	
	Vector2 pos_;																		// ぼたんとの当たり判定用
};

