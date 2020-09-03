#include <DxLib.h>
#include <random>
#include <numeric>
#include <cmath>
#include "SceneMng.h"
#include "../common/ImageMng.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GameEnd.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

uniqueBase TitleScene::Update(uniqueBase own)
{
	// メニューを開く
	if (!menuFlg_)
	{
		ButtonPairVec button;
		int cnt = 0;
		Vector2 tmpPos = { lpSceneMng.screenSize_.x / 2, lpSceneMng.screenSize_.y / 2 + 50};
		Vector2 tmpSize{ 320,64 };

		button.emplace_back(std::make_unique<Button>("StartGame", tmpPos, tmpSize, 1, 0.0f, 10, cnt), NextMap{ Scene::Game, [&]() {return std::make_unique<GameScene>(stage_); } });
		++cnt;
		tmpPos.y += 200;
		button.emplace_back(std::make_unique<Button>("EndGame", tmpPos, tmpSize, 1, 0.0f, 10, cnt), NextMap{ Scene::GameEnd, []() {return std::make_unique<GameEnd>(); } });

		menuFlg_ = true;
		return std::make_unique<MenuScene>(std::move(own), false, true,std::move(button));
	}

	return std::move(own);
}

void TitleScene::Draw(void)
{
	Vector2 tmp{ lpSceneMng.screenSize_ / 2 };
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("BG")[0],tmp.x,tmp.y,1 , 0.0f, 11});


	// 文字ごとに動きを付ける
	Vector2 div{ 5,1 };
	Vector2 tmpPos{ 0 ,0 };
	double tmpRate = 0;
	for (int j = 0; j < div.x; j++)
	{
		int id = lpImageMng.GetHandle("TitleName", div, { 144,124 })[j];

		//tmpPos.y = static_cast<int>(logoPos_.y - 50.0f * cosf((count_ + (j + 1) * 30.0f) * 0.015f));			// サインカーブ
		tmpRate = static_cast<double>(1 + 0.1 * sin((static_cast<double>(count_ + (j + 1.0) * 20) * 0.05)));

		lpSceneMng.AddDrawQue({ id,logoPos_.x + (144 * j),logoPos_.y, tmpRate, 0.0f, 10 });
	}

	count_++;


}

Scene TitleScene::scene()
{
	return scene_;
}

void TitleScene::SetMenuFlg(bool set)
{
	menuFlg_ = set;
}

bool TitleScene::nextUpdate(void)
{
	// フェードインアウトが終わったら次のシーンへ
	if (fadeMode_ == FadeMode::Max)
	{
		return true;
	}

	Vector2 tmp{ lpSceneMng.screenSize_};
	// ステージの描画
	if (stage_.size())
	{
		for (auto&& id : stage_)
		{
			id->DrawStage();
		}
		lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("BG")[0] ,tmp.x / 2,tmp.y / 2, 1,0.0f,9 });
		lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("NextPuyoBG")[0] ,tmp.x / 2, tmp.x / 8,1,0.0f,-1 });
	}

	// フェードの描画
	SetDrawScreen(fadeScreen_);
	ClsDrawScreen();
	int tmpNum = static_cast<int>(FADE_NUM);
	for (int y = 0; y < tmpNum; y++)
	{
		for (int x = 0; x < tmpNum; x++)
		{
			if (imageNum_[y][x])
			{
				int id = lpImageMng.GetHandle("bgbg", { 10,10 }, { 128,90 })[y * 10 + x];

				DrawGraph(128 * x, 90 * y, id, true);
			}
		}
	}
	lpSceneMng.AddDrawQue({ fadeScreen_ ,tmp.x / 2,tmp.y / 2,1.0, 0.0f, -10 });

	//	更新
	fadeMode_ = fadeMap_[fadeMode_]();
	return false;
}


bool TitleScene::Init(void)
{
	menuFlg_ = false;
	scene_ = Scene::Title;
	count_ = 0;
	logoPos_ = { lpSceneMng.screenSize_.x / 4 + 38 ,lpSceneMng.screenSize_.y / 4 };
	fadeScreen_ = MakeScreen(lpSceneMng.screenSize_.x, lpSceneMng.screenSize_.y,true);
	fadeCnt_ = 0;

	// シーン移動用
	imageNum_.resize(static_cast<int>(FADE_NUM));
	for (int y = 0; y < static_cast<int>(FADE_NUM); y++)
	{
		imageNum_[y].resize(static_cast<int>(FADE_NUM));
		for (int x = 0; x < static_cast<int>(FADE_NUM); x++)
		{
			imageNum_[y][x] = false;
		}
	}
	// ランダムの生成
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> posRand(0, 99);

	randomNum_.resize(100);
	std::iota(randomNum_.begin(), randomNum_.end(), 0);				// 0 から100までの値を生成
	std::shuffle(randomNum_.begin(), randomNum_.end(), mt);

	InitFunc();
	return true;
}

bool TitleScene::InitFunc(void)
{
	fadeMode_ = FadeMode::FadeOut;
	waitCnt_ = 0;

	auto fadeOut = [&]()
	{
		// フェードインアウトが終わったら次のシーンへ
		if (fadeCnt_ > 99)
		{
			return FadeMode::Instance;
		}
		// シャッフルされてあるので順番に表示させていく
		int tmpNum = static_cast<int>(FADE_NUM);
		if (!imageNum_[randomNum_[fadeCnt_] / tmpNum][randomNum_[fadeCnt_] % tmpNum])
		{
			imageNum_[randomNum_[fadeCnt_] / tmpNum][randomNum_[fadeCnt_] % tmpNum] = true;
		}
		fadeCnt_++;
		return fadeMode_;
	};

	auto instance = [&]()
	{
		// ステージの生成
		auto scene = GetNextScene();
		if (scene == Scene::Game)
		{
			stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2),
				Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));

			stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameSize_.x * (stage_.size() + 1) + lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2),
				Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));
			return FadeMode::Wait;
		}
		return FadeMode::Max;
	};

	auto wait = [&]()
	{
		if (waitCnt_++ > 120)
		{
			return FadeMode::FadeIn;
		}
		Vector2 tmp{ lpSceneMng.screenSize_ };

		if (GetNextScene() == Scene::Game)
		{
			// よーいどん
			if (waitCnt_ < 80)
			{
				lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("ready")[0] ,tmp.x / 2,tmp.y / 2, 1,0.0f,-11 });
			}
			else
			{
				lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("go")[0] ,tmp.x / 2,tmp.y / 2, 1,0.0f,-11 });
			}
		}

		return fadeMode_;
	};

	auto fadeIn = [&]()
	{
		// フェードインアウトが終わったら次のシーンへ
		if (--fadeCnt_ <= 0)
		{
			return FadeMode::Max;
		}
		// どんどん消していく
		int tmpNum = static_cast<int>(FADE_NUM);
		if (imageNum_[randomNum_[fadeCnt_] / tmpNum][randomNum_[fadeCnt_] % tmpNum])
		{
			imageNum_[randomNum_[fadeCnt_] / tmpNum][randomNum_[fadeCnt_] % tmpNum] = false;
		}

		return fadeMode_;
	};

	fadeMap_.try_emplace(FadeMode::FadeIn, fadeIn);
	fadeMap_.try_emplace(FadeMode::FadeOut, fadeOut);
	fadeMap_.try_emplace(FadeMode::Instance, instance);
	fadeMap_.try_emplace(FadeMode::Wait, wait);

	return true;
}
