#include <DxLib.h>
#include <math.h>
#include <random>
#include "puyo.h"
#include "SceneMng.h"

puyo::puyo(PuyoID id) :size_(64), rad_(size_ / 2, size_ / 2)
{
	Init(id);
}

puyo::~puyo()
{
}

bool puyo::UpDate(void)
{
	if (softCnt_ < softCntMax_)
	{
		softCnt_++;
		return false;
	}
	if (!dirFlg_.bit.down) 
	{
		pos_.y += size_ / 4; 
		softCnt_ = 0;
	}
	else
	{
		return true;
	}

	return false;
}

void puyo::Move(INPUT_ID id)
{
	//// ‰æ–ÊŠO‚És‚Á‚½ê‡‚ÌC³
	switch (id)
	{
	case INPUT_ID::BUTTON_LEFT:
		if (!dirFlg_.bit.left) { pos_.x -= size_; }
		break;
	case INPUT_ID::BUTTON_UP:
		if (!dirFlg_.bit.up) { pos_.y -= size_ / 4; }
		break;
	case INPUT_ID::BUTTON_RIGHT:
		if (!dirFlg_.bit.right) { pos_.x += size_; }
		break;
	case INPUT_ID::BUTTON_DOWN:
		if (!dirFlg_.bit.down) { SoftDrop(); }
		break;
	case INPUT_ID::BUTTON_ROTA:
		break;
	case INPUT_ID::MAX:
		break;
	default:
		break;
	}

}

void puyo::SoftDrop()
{
	softCnt_ = softCntMax_;
}

void puyo::SetSpeed(int spped, int interval)
{
	softCntMax_ = interval;
	speed_ = spped;
}

void puyo::SetPuyon()
{
	puyonCnt_ = 360;
}

void puyo::SetWidth(int width)
{
	width_ = width;
}

bool puyo::CheckPuyon()
{
	return 0 <= puyonCnt_;
}

void puyo::Draw(void)
{
	auto puyonRad = rad_.y + 12 * sinf(puyonCnt_ * 0.02f);
	auto puyonPos = pos_.y + (10 + width_) * sinf(puyonCnt_ * 0.02f);

	if (puyonCnt_ > 0)
	{
		puyonCnt_-= 30;
	}

	DrawOval(pos_.x, puyonPos, rad_.x, puyonRad, puyoCor_[id_], true);
}

bool puyo::SetDirFlg(DirUnion flg)
{
	dirFlg_ = flg;
	return true;
}

void puyo::alive(bool flg)
{
	alive_ =  flg;
}

const Vector2& puyo::pos() const
{
	return pos_;
}

const int puyo::size(void) const
{
	return size_;
}

const Vector2 puyo::rad(void) const
{
	return rad_;
}

const PuyoID puyo::id(void) const
{
	return id_;
}

const Vector2 puyo::GetGrid(int size)
{
	return Vector2((pos_.x + rad_.x) / size, (pos_.y + rad_.y) / size);
}

const bool puyo::alive(void) const
{
	return alive_;
}

void puyo::Init(PuyoID id)
{
	puyoCor_.try_emplace(PuyoID::Red, 0xff0000);
	puyoCor_.try_emplace(PuyoID::Bule,0x0000ff);
	puyoCor_.try_emplace(PuyoID::Green,0x00ff00);
	puyoCor_.try_emplace(PuyoID::Yellow,0xffff00);
	puyoCor_.try_emplace(PuyoID::Purple,0xff00ff);

	pos_ = Vector2(rad_.x + size_ * 3, rad_.y);
	id_ = id;
	softCntMax_ = 20;
	softCnt_ = 0;
	alive_ = true;
	speed_ = size_ / 4;
	puyonCnt_ = 0;
	width_ = 0;
}
