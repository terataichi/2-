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
		ojamaCnt_ = id->Update(ojamaCnt_);				// �X�V
		die &= (!id->alive());							// ���݂������Ă邩�m�F
		nextScene &= id->nextScene();
		// ����ł��烋�[�Y�ɂ���
		if (id->victory() == Victory::Non && !id->alive())
		{
			id->victory(Victory::Lose);
			lose = true;
		}
	}
	
	// lose ��
	if (lose)
	{
		for (auto&& id : stage_)
		{
			// ��������ł���Draw�ɂ���
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

	// true �Ŏ��̃V�[����
	if (nextScene)
	{

		if (oldMenuFlg_ && !menuFlg_)
		{
			StageInit();
			Init();
			oldMenuFlg_ = menuFlg_;
			return own;
		}

		// ���j���[���J��
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

	if (menuFlg_)
	{
		SetDrawScreen(blackScreen_);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		int id = lpImageMng.GetHandle("black2")[0];
		DrawGraph(0, 0, id, true);

		lpSceneMng.AddDrawQue({ blackScreen_ ,tmp.x / 2,tmp.y / 2, 1,0.0f,-99 });
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
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
	blackScreen_ = MakeScreen(lpSceneMng.screenSize_.x, lpSceneMng.screenSize_.y, true);
}

void GameScene::StageInit(void)
{
	stage_.clear();

	stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2),
		Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));

	stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameSize_.x * (stage_.size() + 1) + lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2),
		Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));
}
