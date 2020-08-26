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

	pairPuyo PickUp();				// ねくすとプヨの情報を取り出す
	int screenID();
	void Draw();					// 描画
private:
	bool Add(int no);				// 引数でもらった数だけｲﾝｽﾀﾝｽする

	int screenID_;					// ねくすとプヨの表示用ID
	int drawCnt_;
	static int count_;
	int id_;						// 奇数か偶数で表示位置を変える				
	nextList nextPuyoList_;			// ねくすとプヨリスト

	Vector2 pos_;					// 座標
};

