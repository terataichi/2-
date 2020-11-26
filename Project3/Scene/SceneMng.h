#pragma once
#include <memory>
#include <vector>
#include <chrono>
#include "../common/Vector2.h"
#include "../Scene/LoginScene.h"

#define lpSceneMng SceneMng::GetInstance()

enum class DrawQue
{
	Image,
	X,
	Y,
	EX,						// 拡大倍率
	Angle,
	ZOrder,
};

//						ImageID, 座標,		倍率	角度,	奥行
using drawQueT = std::tuple<int, int, int, double, float, int>;

using chronoTime = std::chrono::system_clock::time_point;

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
	bool AddDrawQue(drawQueT que);

	const chronoTime& GetTime();
	const Vector2 screenSize_;								// スクリーンサイズ
private:
	bool SysInit(void);
	void Init(void);
	void Draw(void);										// 描画

	uniqueBase activeScene_;								// シーン管理

	static SceneMng* sInstance_;

	std::vector<drawQueT>drawList_;							// 描画リスト

	chronoTime time_;

	bool end_;												// 実行条件 true : 続行  false : 実行終了

	SceneMng();
	~SceneMng();
};

