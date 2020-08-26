#include "MenuScene.h"

MenuScene::MenuScene(uniqueBase& child, bool move):childScene_(std::move(child))
{
	move_ = move;
}

MenuScene::~MenuScene()
{
}

uniqueBase MenuScene::Update(uniqueBase own)
{
	// true�ŃA�b�v�f�[�g����
	if (move_)
	{
		(*childScene_).Update(std::move(childScene_));
	}

	return own;
}

void MenuScene::Draw(void)
{
}

void MenuScene::Init(void)
{
}
