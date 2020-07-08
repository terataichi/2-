#pragma once
#include <memory>
#include <vector>
#include "Stage.h"
#include "common/Vector2.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance(void)
	{
		Create();
		return *_sInstance;
	}
	static void Create()
	{
		if (_sInstance == nullptr)
		{
			_sInstance = new SceneMng();
		}
	}
	static void Destroy()									// 呼び忘れに注意
	{
		if (_sInstance != nullptr)
		{
			delete _sInstance;
		}
		_sInstance = nullptr;
	}

	void Run(void);

	const int _pyoSize;										// ぷよの大きさ
	const int _pyoRadius;									// ぷよの半径
	const Vector2 _gameOffSet;								// オフセット
	const Vector2 _gameSize;								// ゲーム画面のみのサイズ
	const Vector2 _frameSize;								// ゲーム画面の周りのフレームのサイズ
	const Vector2 _screenSize;								// スクリーンサイズ
private:
	bool SysInit(void);
	void Init(void);
	void Draw(void);										// 描画

	std::vector<std::unique_ptr<Stage>> _stage;				// ステージを配列で管理して人数変更を可能にする
	static SceneMng* _sInstance;

	SceneMng();
	~SceneMng();
};

