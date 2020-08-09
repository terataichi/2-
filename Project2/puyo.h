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
	Ojama,
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
	puyo(Vector2&& pos,PuyoID id) ;
	~puyo();
	virtual bool UpDate(int no);
	void Move(INPUT_ID id);										// 移動関数
	void SoftDrop();											// ソフトドロップ
	bool CheckPuyon(void);										// まだぷよんしてるか確認する
	bool CheckMunyon(void);										// むにょんカウントチェックー
	void Draw(void);											// 描画
	
	// ------ セット関数
	bool SetDirFlg(DirUnion flg);								// 移動フラグセット用
	bool SetOldDirFlg(void);									// 移動フラグの前の情報を格納
	bool SetMnyonFlg(DirUnion flg);								// むにょんしていいかの情報をセット
	void alive(bool flg);
	void SetCnt(int cnt);										// 下の方が深い
	void SetSpeed(int spped, int interval);						// 連鎖とかでスピードを変える
	void SetPuyon(void);										// 呼んだらぷよん開始
	void SetMunyon(int cnt);									// 呼んだらむにょん開始
	void pos(Vector2& vec);										// 座標
	void UpSpeed();												// 加速させる
	virtual void SetFall(bool flg);								// お邪魔用、落ちていいかどうか
	// ------ゲット関数
	virtual bool seiretu(void);									// お邪魔用、整列用
	const Vector2& pos(void)const;
	const DirUnion& GetDirFlg(void)const;
	const DirUnion& GetOldDirFlg(void)const;
	const int size(void)const;
	const Vector2 rad(void)const;
	const PuyoID id(void)const;
	const Vector2 GetGrid(int size);							// 現在のマス目の取得
	const Vector2 GetGrid(Vector2 pos, int size);				// 指定した場所のマス目の取得
	const bool alive(void)const;
protected:
	void Init(Vector2& pos,PuyoID id);

	const int size_;											// ぷよのサイズ
	const Vector2 rad_;											// ぷよの半径
	Vector2 pos_;
	DirUnion dirFlg_;											// 移動していいのかどうかの情報をセットする
	DirUnion oldDirFlg_;										// 前の情報
	DirUnion mnyonFlg_;											// むにょんフラグ、trueだったら横に伸ばす
	PuyoID id_;													// 自分のぷよの識別用
	int softCnt_;												// 自動落下用
	int softCntMax_;											// 自動落下の最大
	bool alive_;												// 生きてるかどうか
	int speed_;													// ぷよの速さ
	
	bool speedUp_;

	int puyonOffset_;											// ぷよんするときの振れ幅
	int puyonCnt_;												// ぷよんカウント

	int munyonCnt_;												// むにょんカウント

	std::map<PuyoID, int> puyoCor_;
};

