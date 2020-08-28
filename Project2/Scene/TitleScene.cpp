#include <DxLib.h>
#include "SceneMng.h"
#include "../common/ImageMng.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

uniqueBase TitleScene::Update(uniqueBase own)
{
	// ÉÅÉjÉÖÅ[ÇäJÇ≠
	if (!flg)
	{
		ButtonPairVec button;

		for (int j = 0; j < 2; j++)
		{
			Vector2 tmpPos = lpSceneMng.screenSize_ / 2;
			Vector2 tmpSize{ 320,64 };
			tmpPos.y += (200 * j);
			button.emplace_back(std::make_unique<Button>(std::string("win").c_str(), tmpPos, tmpSize, 1, 0.0f, -100, j), std::make_unique<GameScene>());
		}
		flg = true;
		return std::make_unique<MenuScene>(std::move(own), true, true,std::move(button));
	}

	return std::move(own);
}

void TitleScene::Draw(void)
{
	Vector2 tmp{ lpSceneMng.screenSize_ / 2 };
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("BG")[0],tmp.x,tmp.y,1 , 0.0f, 0});
}

bool TitleScene::Init(void)
{
	return false;
}
