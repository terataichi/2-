#pragma once
#include <memory>

class BaseScene;

using uniqueBase = std::unique_ptr<BaseScene>;

// 各シーン共通のメンバ
class BaseScene
{
public:
	BaseScene();											// コンストラクタ
	virtual ~BaseScene();									// 継承先のデストラクタを呼ぶ
	virtual uniqueBase Update(uniqueBase scene) = 0;		// 引数は今のシーンを渡す、シーンを変えるときは返り値で
	virtual void Draw();									// 各シーンのDraw
	virtual void DrawOwnScene() = 0;						// 							
	virtual void Init() = 0;								// 初期化

protected:

	static int count_;

	int drawScreen_;										// 各シーンの描画用
};

