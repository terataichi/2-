#include <DxLib.h>
#include "MouseCtl.h"
#include "_debug/_DebugConOut.h"

MouseCtl::MouseCtl()
{
	pos = { 0,0 };
	mouseData = 0;
	mouseDataOld = 0;
	TRACE("MouseCtlÇÃê∂ê¨");
}

MouseCtl::~MouseCtl()
{
	TRACE("MouseCtlÇÃîjä¸");
}

bool MouseCtl::GetClickTrg(void)
{
	return (mouseData == MOUSE_INPUT_LEFT) && !mouseDataOld;
}

bool MouseCtl::GetClicking(void)
{
	return (mouseData == MOUSE_INPUT_LEFT) && mouseDataOld;
}

const Vector2& MouseCtl::GetPos(void)
{
	return pos;
}

void MouseCtl::Update(void)
{
	GetMousePoint(&pos.x, &pos.y);

	mouseDataOld = mouseData;
	mouseData = GetMouseInput();
}
