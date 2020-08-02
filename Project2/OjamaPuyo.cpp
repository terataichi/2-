#include "OjamaPuyo.h"


OjamaPuyo::OjamaPuyo(Vector2&& pos, PuyoID id)
{
	Init(pos,id);
	softCntMax_ = 0;
}

OjamaPuyo::~OjamaPuyo()
{
}

//bool OjamaPuyo::UpDate()
//{
//	return false;
//}
