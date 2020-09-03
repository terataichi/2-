#include "GameScene.h"
#include <DxLib.h>
#include "SceneMng.h"
#include "GameOverScene.h"
#include "MenuScene.h"
#include "TitleScene.h"
#include "GameEnd.h"
#include "../common/ImageMng.h"


GameScene::GameScene(StageVec& stage) :stage_(std::move(stage))
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

		if (oldMenuFlg_ && !menuFlg_)
		{
			StageInit();
			Init();
			oldMenuFlg_ = menuFlg_;
			return own;
		}

		// メニューを開く
		if (!menuFlg_)
		{
			ButtonPairVec button;
			int cnt = 0;

			Vector2 tmpPos = { lpSceneMng.screenSize_.x / 2,lpSceneMng.screenSize_.y / 3 };
			Vector2 tmpSize{ 320,64 };

			button.emplace_back(std::make_unique<Button>("Continue", tmpPos, tmpSize, 1, 0.0f, -100, cnt), NextMap{ Scene::Game, [&]() {return std::make_unique<GameScene>(stage_); } });
			++cnt;
			tmpPos.y += 200;
			button.emplace_back(std::make_unique<Button>("GoTitle", tmpPos, tmpSize, 1, 0.0f, -100, cnt), NextMap{ Scene::Title, []() {return std::make_unique<TitleScene>(); } });
			++cnt;
			tmpPos.y += 200;
			button.emplace_back(std::make_unique<Button>("EndGame", tmpPos, tmpSize, 1, 0.0f, -100, cnt), NextMap{ Scene::GameEnd, []() {return std::make_unique<GameEnd>(); } });
			menuFlg_ = true;
			oldMenuFlg_ = menuFlg_;
			return std::make_unique<MenuScene>(std::move(own), true, true, std::move(button));
		}
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
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("NextPuyoBG")[0] ,tmp.x / 2, tmp.x / 8,1,0.0f,-1 });
}

Scene GameScene::scene()
{
	return scene_;
}

void GameScene::SetMenuFlg(bool set)
{
	menuFlg_ = set;
}

void GameScene::Init(void)
{
	oldMenuFlg_ = false;
	scene_ = Scene::Game;
	menuFlg_ = false;
	ojamaCnt_ = 0;
}

void GameScene::StageInit(void)
{
	stage_.clear();

	stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2),
		Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));

	stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameSize_.x * (stage_.size() + 1) + lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2),
		Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));
}
