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

class Button
{
public:
	//		画像ファイル名　	座標　　　　サイズ　　　　拡大率　　　角度		描画優先度	何個目のボタン？
	Button(std::string name, Vector2& pos, Vector2& size,int rate, float angle, int zLayer, int num);
	~Button();

	void Update();
	bool CheckHitButton(Vector2 pos);				// ボタンとの当たり判定
	void Draw();									// 描画
	void reset();									// 座標など設定しなおす

	// ----- Set
	void pos(Vector2 pos);
	void angle(float angle);
	void movePattern(MovePattern pattern);
	void exRate(int exRate);
	// ----- Get
	const Vector2& pos(void)const;
	const Vector2& stdPos(void)const;
	const float& angle(void)const;

private:
	bool Init(std::string name,Vector2& pos, Vector2& size, int& rate, float angle,int zLayer,int num);		// 初期化
	bool InitFunc(void);																					// ファンクションの初期化

	Vector2 pos_;																							// buttonの座標
	Vector2 size_;																							// buttonのサイズ
	float angle_;																							// buttonの角度
	double exRate_;																							// 拡大率

	Vector2 stdPos_;																						// 座標の基準位置
	float stdAngle_;																						// 角度の基準位置
	double stdExRate_;																						// 拡大率の基準
	int zLayer_;																							// ゼットレイヤー

	std::string id_;																						// buttonになる画像のID格納用

	MovePattern oldPattern_;																				// 前情報格納用
	MovePattern movePattern_;																				// 動きのパターン識別用
	int moveCnt_;																							// 動き専用カウント

	int buttonID_;																							// 上から何番目？

	std::map<MovePattern, std::function<void(void)>> moveFunc;												// 各動き対応の関数オブジェクト
};

