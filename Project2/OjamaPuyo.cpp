#include "OjamaPuyo.h"

int OjamaPuyo::ojamaCnt_ = 0;

OjamaPuyo::OjamaPuyo(Vector2&& pos, PuyoID id)
{
	Init(pos,id);
	id_ = ojamaCnt_;
	ojamaCnt_++;

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
	if (seiretu_ && fall_)
	{
		intervalCnt_++;
	}

	// ®—ñ‚µ‚«‚Á‚Ä‚¢‚½‚ç‚±‚Á‚¿‰ñ‚»‚¤
	if (fall_ && intervalCnt_ > interval_)
	{
		return 	puyo::UpDate(0);
	}

	// ®—ñ‚³‚¹‚é
	if (pos_.x < ((no % 6) * size_) + rad_.x)
	{
		pos_.x += 8;
		return false;
	}

	if (pos_.x > ((no % 6) * size_) + rad_.x)
	{
		pos_.x -= 8;
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

//bool OjamaPuyo::UpDate()
//{
//	return false;
//}
