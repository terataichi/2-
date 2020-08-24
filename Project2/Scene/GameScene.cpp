#include "GameScene.h"
#include "../Scene/SceneMng.h"
#include "../common/ImageMng.h"
#include "../Scene/GameOverScene.h"

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

	if (nextScene)
	{
		return std::make_unique<GameOverScene>();
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

	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("BG")[0] ,lpSceneMng.screenSize_ / 2,0.0f,1000 });
	return std::move(own);
}

void GameScene::Draw()
{
	for (auto&& id : stage_)
	{
		id->DrawStage();
	}
}

void GameScene::Init(void)
{
	stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2), 
												Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));

	stage_.emplace_back(std::make_unique<Stage>(Vector2(lpSceneMng.gameSize_.x * (stage_.size() + 1) + lpSceneMng.gameOffSet_.x, lpSceneMng._frameSize.y / 2),
												Vector2(lpSceneMng.gameSize_.x, lpSceneMng.gameSize_.y)));
	ojamaCnt_ = 0;
}
