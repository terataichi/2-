#include "OjamaPuyo.h"


OjamaPuyo::OjamaPuyo(Vector2&& pos, PuyoID id)
{
	Init(pos,id);
	softCntMax_ = 0;
	seiretuCnt_ = 0;
	seiretu_ = false;
}

OjamaPuyo::~OjamaPuyo()
{
}

void OjamaPuyo::seiretuCnt(int cnt)
{
	seiretuCnt_ = cnt;
}

bool OjamaPuyo::UpDate(void)
{
	// ���񂵂����Ă����炱�����񂻂�
	if (seiretu_)
	{
		return 	puyo::UpDate();
	}

	//// ���񂳂���
	if (pos_.x < )
	{
		pos_.x += 4;
	}

	if (pos_.x > )
	{
		pos_.x -= 4;
	}
	
	return false;
}

//bool OjamaPuyo::UpDate()
//{
//	return false;
//}
