#pragma once
#include <map>
#include "common/Vector2.h"
#include "Input/INPUT_ID.h"

enum class PuyoState
{
	Drop,		// 落ちてる
	Stop,		// 止まる
	Die,		// 消える
	Max
};

enum class PuyoID
{
	Non,
	Red,
	Green,
	Bule,
	Yellow,
	Purple,
	Wall,
	Max,
};

// 方向の入力のビットフィールド
struct DirBits
{
	unsigned int left : 1;			// 左
	unsigned int up : 1;			// 上
	unsigned int right : 1;			// 右
	unsigned int down : 1;			// 下
};

union DirUnion
{
	DirBits bit;
	unsigned int dirFlg;
};

class puyo
{
public:
	puyo();
	~puyo();
	bool UpDate(void);
	void Move(INPUT_ID id);
	void SoftDrop();
	void Draw(void);
	
	bool SetDirFlg(DirUnion flg);
	void alive(bool flg);

	// ------ゲット関数
	const Vector2& pos(void)const;
	const int size(void)const;
	const int rad(void)const;
	const PuyoID id(void)const;
	const Vector2 GetGrid(int size);							// 現在のマス目の取得
	const bool alive(void)const;
private:
	void Init(void);

	const int size_;											// ぷよのサイズ
	const int rad_;												// ぷよの半径
	Vector2 pos_;
	DirUnion dirFlg_;											// 移動していいのかどうかの情報をセットする
	PuyoID id_;													// 自分のぷよの識別用
	int softCnt_;												// 自動落下用
	int softCntMax_;											// 自動落下の最大
	bool alive_;												// 生きてるかどうか
	
	std::map<PuyoID, int> puyoCor_;
};

