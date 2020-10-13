#include <DxLib.h>
#include "Scene/SceneMng.h"

#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	lpSceneMng.Run();
	DxLib_End();
	return 0;
}