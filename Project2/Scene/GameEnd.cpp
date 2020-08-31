#include "GameEnd.h"

GameEnd::GameEnd()
{
}

GameEnd::~GameEnd()
{
}

uniqueBase GameEnd::Update(uniqueBase own)
{
	lpSceneMng.SetEnd();
	return own;
}

void GameEnd::Draw(void)
{
}
