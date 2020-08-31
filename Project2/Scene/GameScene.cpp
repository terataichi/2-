#include "GameScene.h"
#include <DxLib.h>
#include "SceneMng.h"
#include "GameOverScene.h"
#include "MenuScene.h"
#include "../common/ImageMng.h"


GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

uniqueBase GameScene::Update(uniqueBase own)
{
	bool die = true;
	bool lose = false;
	bool nextScene = true;
	for (auto&& id : stage_)
	{
		ojamaCnt_ = id->Update(ojamaCnt_);				// 更新
		die &= (!id->alive());							// お互い生きてるか確認
		nextScene &= id->nextScene();
		// 死んでたらルーズにする
		if (id->victory() == Victory::Non && !id->alive())
		{
			id->victory(Victory::Lose);
			lose = true;
		}
	}
	
	// lose の
	if (lose)
	{
		for (auto&& id : stage_)
		{
			// 両方死んでたらDrawにする
			if (die)
			{
				id->victory(Victory::Draw);
			}
			else
			{
				if (id->victory() == Victory::Non)
				{
					id->victory(Victory::Win);
				}
			}
		}
	}

	// true で次のシーンへ
	if (nextScene)
	{
		int screenShot = CreateMaskScreen();
		return std::make_unique<GameOverScene>(screenShot);
	}

	//// メニューを開く
	//if (CheckHitKey(KEY_INPUT_F2))
	//{
	//	ButtonVec button;

	//	for()

	//	return std::make_unique<MenuScene>(own,false,);
	//}

	return std::move(own);
}

void GameScene::Draw()
{
	for (auto&& id : stage_)
	{
		id->DrawStage();
	}

	Vector2 tmp = lpSceneMng.screenSize_;
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("BG")[0] ,tmp.x / 2,tmp.y / 2, 1,0.0f,1000 });
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("NextBG")[0] ,tmp.x / 2, tmp.x / 8,1,0.0f,-1 });
}

void GameScene::Init(void)
{
	stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2), 
												Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));

	stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameSize_.x * (stage_.size() + 1) + lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2),
												Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));
	ojamaCnt_ = 0;
}
