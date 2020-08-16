#pragma once
#include <memory>
#include <vector>
#include "../Stage.h"
#include "../common/Vector2.h"
#include "../Scene/BaseScene.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance(void)
	{
		Create();
		return *sInstance_;
	}
	static void Create()
	{
		if (sInstance_ == nullptr)
		{
			sInstance_ = new SceneMng();
		}
	}
	static void Destroy()									// 呼び忘れに注意
	{
		if (sInstance_ != nullptr)
		{
			delete sInstance_;
		}
		sInstance_ = nullptr;
	}

	void Run(void);

	const int pyoSize_;										// ぷよの大きさ
	const int pyoRadius_;									// ぷよの半径
	const Vector2 gameOffSet_;								// オフセット
	const Vector2 gameSize_;								// ゲーム画面のみのサイズ
	const Vector2 _frameSize;								// ゲーム画面の周りのフレームのサイズ
	const Vector2 screenSize_;								// スクリーンサイズ
private:
	bool SysInit(void);
	void Init(void);
	void Draw(void);										// 描画

	uniqueBase activeScene_;								// シーン管理

	static SceneMng* sInstance_;

	SceneMng();
	~SceneMng();
};

