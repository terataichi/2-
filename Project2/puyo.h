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
	void UpDate(void);
	void Move(INPUT_ID id);
	void Draw(void);
	
	bool SetDirFlg(DirUnion flg);
	// ------ゲット関数
	const Vector2 pos(void)const;
	const Vector2 size(void)const;
	const Vector2 rad(void)const;
private:
	void Init(void);

	const Vector2 _size;				// ぷよのサイズ
	const Vector2 _rad;					// ぷよの半径
	std::map<INPUT_ID, Vector2> _vec;	// キーに対しての移動量格納
	Vector2 _pos;
	DirUnion _dirFlg;					// 移動していいのかどうかの情報をセットする
};

