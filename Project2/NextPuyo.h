#pragma once
#include <memory>
#include <list>
#include "common/Vector2.h"
#include "puyo.h"
#include "Stage.h"

using pairPuyo = std::pair<SharePuyo, SharePuyo>;
using nextList = std::list<pairPuyo>;

class NextPuyo
{
public:
	NextPuyo(Vector2& pos, int drawCount);
	~NextPuyo();

	pairPuyo PickUp();				// ネクストぷよの情報を取り出す
	int screenID();
	void Draw();					// 描画
private:							// プライベート
	bool Add(int no);				// 引数でもらった数だけインスタンスする

	int screenID_;					// ネクストぷよの表示用ID
	int drawCnt_;
	static int count_;
	int id_;						// 偶数と奇数で表示場所を変えたい					
	nextList nextPuyoList_;			// ネクストぷよリスト

	Vector2 pos_;					// 座標
};

