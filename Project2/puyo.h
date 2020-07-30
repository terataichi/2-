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
	puyo(Vector2&& pos,PuyoID id);
	~puyo();
	bool UpDate(void);
	void Move(INPUT_ID id);										// 移動関数
	void SoftDrop();											// ソフトドロップ
	bool CheckPuyon();											// まだぷよんしてるか確認する
	void Draw(void);											// 描画
	
	// ------ セット関数
	bool SetDirFlg(DirUnion flg);
	bool SetOldDirFlg(void);
	void alive(bool flg);
	void SetWidth(int width);									// 下の方が深い
	void SetSpeed(int spped, int interval);						// 連鎖とかでスピードを変える
	void SetPuyon(void);										// 呼んだらぷよん開始
	void SetMunyon(void);										// 呼んだらむにょん開始
	void pos(Vector2& vec);										// 座標
	void UpSpeed();												// 加速させる

	// ------ゲット関数
	const Vector2& pos(void)const;
	const DirUnion& GetDirFlg(void)const;
	const DirUnion& GetOldDirFlg(void)const;
	const int size(void)const;
	const Vector2 rad(void)const;
	const PuyoID id(void)const;
	const Vector2 GetGrid(int size);							// 現在のマス目の取得
	const Vector2 GetGrid(Vector2 pos, int size);				// 指定した場所のマス目の取得
	const bool alive(void)const;
private:
	void Init(Vector2& pos,PuyoID id);

	const int size_;											// ぷよのサイズ
	const Vector2 rad_;											// ぷよの半径
	Vector2 pos_;
	DirUnion dirFlg_;											// 移動していいのかどうかの情報をセットする
	DirUnion oldDirFlg_;										// 前の情報
	PuyoID id_;													// 自分のぷよの識別用
	int softCnt_;												// 自動落下用
	int softCntMax_;											// 自動落下の最大
	bool alive_;												// 生きてるかどうか
	int speed_;													// ぷよの速さ
	
	bool up_ = false;

	int width_;													// ぷよんするときの振れ幅
	int puyonCnt_;												// ぷよんカウント

	std::map<PuyoID, int> puyoCor_;
};

