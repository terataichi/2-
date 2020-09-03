#pragma once
#include <memory>

class BaseScene;

enum class Scene
{
	Non,
	Base,
	Title,
	Game,
	GameOver,
	GameEnd,
	Menu,
};

enum class FadeMode
{
	FadeOut,
	Instance,
	Wait,
	FadeIn,
	Max,
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
	virtual bool nextUpdate(void);							// ボタンが押された後に回すアップデート
	void SetNextScene(Scene);
	Scene GetNextScene(void);

protected:

	bool menuFlg_;											// メニューが開かれているかどうか
	Scene scene_;											// 自分のシーンを格納
private:
	Scene nextScene_;										// 次のシーンが決まったら設定する
};

