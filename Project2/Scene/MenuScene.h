#pragma once
#include <vector>
#include "BaseScene.h"
#include "../common/Button.h"
#include "../Input/InputState.h"

enum class NextScene
{
	Title,
	Game,
	GameOver,
	GameEnd,
};

using uniqueButton = std::unique_ptr<Button>;

// ぼたんに対応したシーンを格納する
using ButtonPairVec = std::vector<std::pair<uniqueButton, NextScene>>;

// 次のシーンを生成するためのマップ
using NextMap = std::map < NextScene, std::function<uniqueBase(void)> >;

// 各操作方法に応じて座標の替え方が違う
using InputPosP = std::pair<std::shared_ptr<InputState>, std::function<void(void)>>;

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
	void InitFunc(void);

	bool childUpdateFlg_;												// メニューで子のアップデートを動かすか動かさないか
	bool childDrawFlg_;													// メニューで子の描画をするかどうか
	uniqueBase childScene_;												// メニューを開いたときに前のシーンを保存しておく

	ButtonPairVec buttonVec_;											// ぼたん管理用
	NextMap nextMap_;													// ぼたんに対応したシーンの格納

	std::map<CntType, InputPosP> input_;								// キーの入力管理
	int buttonCnt_;														// ぼたんの番号
	int oldButtonCnt_;													// 前のボタンの番号

	Vector2 pos_;														// ぼたんとの当たり判定用
};

