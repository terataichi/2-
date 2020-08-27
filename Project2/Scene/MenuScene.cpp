#include "MenuScene.h"

/// <summary>
/// 
/// </summary>
/// <param name="child"></param>
/// <param name="move"></param>
/// <param name="buttonNum"></param>
/// <param name="width"></param>
/// <returns></returns>
MenuScene::MenuScene(uniqueBase& child, bool move, int buttonNum, Vector2&& buttonPos, Vector2&& size):childScene_(std::move(child))
{
	Init(move, buttonNum, buttonPos, size);
}

MenuScene::~MenuScene()
{
}

uniqueBase MenuScene::Update(uniqueBase own)
{
	// trueでアップデートを回す
	if (move_)
	{
		(*childScene_).Update(std::move(childScene_));
	}

	return own;
}

void MenuScene::Draw(void)
{
}

void MenuScene::Init(bool move, int buttonNum, Vector2& buttonPos, Vector2& size)
{
	move_ = move;
	
	for (int no = 0; no < buttonNum; no++)
	{
		button_.emplace_back(std::make_unique<Button>());
	}
}
