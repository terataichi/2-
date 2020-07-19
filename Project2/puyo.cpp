#include <DxLib.h>
#include <random>
#include "puyo.h"
#include "SceneMng.h"

puyo::puyo() :size_(64), rad_(size_ / 2)
{
	Init();
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
	//// ��ʊO�ɍs�����ꍇ�̏C��
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

void puyo::Draw(void)
{
	DrawCircle(pos_.x, pos_.y, rad_, puyoCor_[id_], true);
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

const int puyo::rad(void) const
{
	return rad_;
}

const PuyoID puyo::id(void) const
{
	return id_;
}

const Vector2 puyo::GetGrid(int size)
{
	return Vector2((pos_.x + rad_) / size, (pos_.y + rad_) / size);
}

const bool puyo::alive(void) const
{
	return alive_;
}

void puyo::Init(void)
{
	puyoCor_.try_emplace(PuyoID::Red, 0xff0000);
	puyoCor_.try_emplace(PuyoID::Bule,0x0000ff);
	puyoCor_.try_emplace(PuyoID::Green,0x00ff00);
	puyoCor_.try_emplace(PuyoID::Yellow,0xffff00);
	puyoCor_.try_emplace(PuyoID::Purple,0xff00ff);

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> puyoRand(static_cast<int>(PuyoID::Red), static_cast<int>( PuyoID::Purple));

	pos_ = Vector2(rad_ + size_ * 3, rad_);
	id_ = static_cast<PuyoID>(puyoRand(mt));
	softCntMax_ = 20;
	softCnt_ = 0;
	alive_ = true;
}
