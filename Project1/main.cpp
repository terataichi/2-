#include <Dxlib.h>
#include "main.h"
#include "MySelf.h"
#include "TicketMachine.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	MySelf::GetInstance().Run();
	MySelf::Destroy();

	DxLib_End();
	return 0;
}
