#include "OjamaPuyo.h"


OjamaPuyo::OjamaPuyo(Vector2&& pos, int no)
{
	Init(pos, PuyoID::Ojama);
	id_ = no;

	intervalCnt_ = 0;
	interval_ = (id_ / 6) * 4;
	softCntMax_ = 0;
	seiretuCnt_ = 0;
	seiretu_ = false;
	fall_ = false;
}

OjamaPuyo::~OjamaPuyo()
{
}

void OjamaPuyo::seiretuCnt(int cnt)
{
	seiretuCnt_ = cnt;
}

bool OjamaPuyo::UpDate(int no)
{

	// ���񂵂����Ă����炱�����񂻂�
	if (fall_  && drop_)
	{
		intervalCnt_++;

		if (intervalCnt_ > interval_)
		{
			return 	puyo::UpDate(0);
		}
		return false;
	}

	if (seiretu_)
	{
		return false;
	}

	// ���񂳂���
	if (pos_.x < ((id_ % 6) * size_) + rad_.x)
	{
		pos_.x += 16;
		return false;
	}

	if (pos_.x > ((id_ % 6) * size_) + rad_.x)
	{
		pos_.x -= 16;
		return false;
	}
	
	
	seiretu_ = true;
	return false;
}

bool OjamaPuyo::seiretu(void)
{
	return seiretu_;
}

void OjamaPuyo::SetFall(bool flg)
{
	fall_ = flg;
}

bool OjamaPuyo::CheckFall()
{
	return fall_;
}

void OjamaPuyo::SetDrop()
{
	drop_ = true;
}