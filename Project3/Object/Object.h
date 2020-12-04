#pragma once
#include"../common/Vector2.h"
#include "../NetWork/NetWork.h"
#include "../common/TileMap.h"
#include "../Scene/SceneMng.h"
#include "../DIR.h"

#define UNIT_ID_BASE 5			// ボムが出せる最大数


enum class STATE
{
	Non,
	Run,
	Deth,
};

enum class ObjectType
{
	Non,
	Player,
	Bomb,
	Item,
	Max
};

#define MAX_FIRE_LENGTH 6

class Object
{
public:
	Object();
	virtual ~Object() = default;

	const int ID(void) { return id_; };
	bool Alive(void) { return alive_; };

	bool CheckData(MesType type);
	virtual bool Update() = 0;
	virtual void Draw(void) = 0;

	virtual bool UpdateDef() = 0;					// 入力処理管理
	virtual bool UpdateAuto() { return true; };		// オートパイロット
	virtual bool UpdateRev() { return true; };		// 受信

	virtual ObjectType ObjType() { return ObjectType::Non; };
protected:

	std::mutex revMutex_;
	RevDataListP revList_;

	void PickData(MesType type, UnionVec& vec);
	virtual void AnimStateInit(void);

	Vector2 pos_;
	float rad_;
	Vector2 vel_;
	static int count;
	int id_;
	bool alive_;

	STATE state_;
	DIR dir_;

	std::map<STATE, std::vector<int>>animState_;				// アニメーション別ベクター

	int animCnt_;
};

