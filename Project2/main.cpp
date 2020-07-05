#include <DxLib.h>
#include "SceneMng.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	lpSceneMng.Run();
	lpSceneMng.Destroy();
	DxLib_End();
	return 0;
}