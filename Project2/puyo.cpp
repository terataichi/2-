#include <DxLib.h>
#include <math.h>
#include <random>
#include "puyo.h"
#include "SceneMng.h"
#include "_debug/_DebugDispOut.h"

puyo::puyo(Vector2&& pos,PuyoID id) :size_(64), rad_(size_ / 2, size_ / 2)
{
	Init(pos, id);
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
		if (up_)	// 自動落下中は少しずつ早くなる
		{
			if (speed_ < size_ / 4)
			{
				speed_ += 2;
			}
		}
		pos_.y += speed_;
		softCnt_ = 0;
	}
	else
	{
		// 着地したときにあまりが出たら引き戻す
		auto amari = (pos_.y + rad_.y) % size_;
		if (amari != 0)
		{
			pos_.y -= amari;
		}
		return true;
	}

	return false;
}

void puyo::Move(INPUT_ID id)
{
	//// 画面外に行った場合の修正
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
		break;
	case INPUT_ID::BUTTON_ROTA_L:
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

void puyo::pos(Vector2& vec)
{
	pos_ = vec;
}

void puyo::UpSpeed()
{
	up_ = true;
}

void puyo::SetPuyon()
{
	puyonCnt_ = 180;
}

void puyo::SetMunyon(void)
{
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
	auto puyonRad = rad_.y - 12 * sinf(puyonCnt_ * 0.02f);
	auto puyonPos = pos_.y + (12 + width_) * sinf(puyonCnt_ * 0.01f);

	if (puyonCnt_ > 0)
	{
		puyonCnt_-= 15;
	}

	DrawOval(pos_.x, puyonPos, rad_.x, puyonRad, puyoCor_[id_], true);
}

bool puyo::SetDirFlg(DirUnion flg)
{
	dirFlg_ = flg;
	return true;
}

bool puyo::SetOldDirFlg(void)
{
	oldDirFlg_ = dirFlg_;
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

const DirUnion& puyo::GetDirFlg()const
{
	return dirFlg_;
}

const DirUnion& puyo::GetOldDirFlg(void)const
{
	return oldDirFlg_;
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

const Vector2 puyo::GetGrid(Vector2 pos, int size)
{
	return Vector2((pos.x + rad_.x) / size, (pos.y + rad_.y) / size);
}

const bool puyo::alive(void) const
{
	return alive_;
}

void puyo::Init(Vector2& pos,PuyoID id)
{
	puyoCor_.try_emplace(PuyoID::Red, 0xff0000);
	puyoCor_.try_emplace(PuyoID::Bule,0x0000ff);
	puyoCor_.try_emplace(PuyoID::Green,0x00ff00);
	puyoCor_.try_emplace(PuyoID::Yellow,0xffff00);
	puyoCor_.try_emplace(PuyoID::Purple,0xff00ff);

	pos_ = pos;
	id_ = id;
	softCntMax_ = 20;
	softCnt_ = 0;
	alive_ = true;
	speed_ = size_ / 4;
	puyonCnt_ = 0;
	width_ = 0;
}
