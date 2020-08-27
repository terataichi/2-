#pragma once
#include <map>
#include <functional>
#include <string>
#include "Vector2.h"

// 動きのパターンを決めておく
enum class MovePattern
{
	Non,						// 動かない
	UpDown,						// 上下に動く
	LeftRight,					// 左右に動く
	Scale,						// 拡大縮小
};

using ButtonMoveFunc = std::function<void(Vector2& pos, float& angle,int& exRate, int count)>;

class Button
{
public:
	//			画像ファイル名　座標　　　　サイズ　　　　拡大率　　　角度
	Button(std::string& name, Vector2&& pos, Vector2&& size,int rate, float angle);
	~Button();

	void Update();
	bool CheckHitButton(Vector2 pos);				// ボタンとの当たり判定

	// ----- Set
	void pos(Vector2 pos);
	void angle(float angle);
	void movePattern(MovePattern pattern);
	void exRate(int exRate);
	// ----- Get
	const Vector2& pos(void)const;
	const float& angle(void)const;
private:
	bool Init(std::string name,Vector2 pos, Vector2 size, int rate, float angle);
	void Draw();

	Vector2 pos_;											// buttonの座標
	Vector2 size_;											// buttonのサイズ
	float angle_;											// buttonの角度
	int exRate_;											// 拡大率

	Vector2 stdPos_;										// 座標の基準位置
	float stdAngle_;										// 角度の基準位置
	int stdExRate_;											// 拡大率の基準

	std::string id_;										// buttonになる画像のID格納用

	MovePattern oldPattern_;								// 前情報格納用
	MovePattern movePattern_;								// 動きのパターン識別用
	int moveCnt_;											// 動き専用カウント

	std::map<MovePattern, ButtonMoveFunc> moveFunc;			// 各動き対応の関数オブジェクト
};

