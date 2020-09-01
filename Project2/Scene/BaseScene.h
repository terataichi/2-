#pragma once
#include <memory>

class BaseScene;

enum class Scene
{
	Base,
	Title,
	Game,
	GameOver,
	GameEnd,
	Menu,
};

using uniqueBase = std::unique_ptr<BaseScene>;

// 各シーン共通のメンバ
class BaseScene
{
public:
	BaseScene();											// コンストラクタ
	virtual ~BaseScene();									// 継承先のデストラクタを呼ぶ
	virtual uniqueBase Update(uniqueBase scene) = 0;		// 引数は今のシーンを渡す、シーンを変えるときは返り値で
	virtual void Draw() = 0;								// 各シーンのDraw
	virtual Scene scene() = 0;								// 自分が何のシーンなのか識別
	virtual void SetMenuFlg(bool set);						// メニューが開かれているかどうかをセットする

protected:
	bool menuFlg_;
	Scene scene_;
};

