#include <DxLib.h>
#include <math.h>
#include <random>
#include "puyo.h"
#include "Scene/SceneMng.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

puyo::puyo() :size_(64), rad_(size_ / 2, size_ / 2)
{
	//TRACE("基底のデフォルトコンストラクタ\n");
}

puyo::puyo(Vector2&& pos,PuyoID id) :size_(64), rad_(size_ / 2, size_ / 2)
{
	Init(pos, id);
}

puyo::~puyo()
{
}

bool puyo::UpDate(int no)
{
	// ぷよんの更新
	//PuyonUpdate();

	if (softCnt_ < softCntMax_)
	{
		softCnt_++;
		return false;
	}

	if (!dirFlg_.bit.down) 
	{
		// 自動落下中は少しずつ早くなる
		if (speedUp_)
		{
			if (speed_ < size_ / 2)
			{
				speed_ += 1;
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
	speedUp_ = true;
}

bool puyo::CheckFall()
{
	return false;
}

void puyo::SetDrop()
{
}

void puyo::SetFall(bool flg)
{
}

void puyo::SetPuyon()
{
	puyonCnt_ = 12;
}

void puyo::SetMunyon(int cnt)
{
	munyonCnt_ = cnt;
}

void puyo::SetCnt(int cnt)
{
	puyonOffset_ = cnt;
}

bool puyo::CheckPuyon(void)
{
	return 0 >= puyonCnt_;
}

bool puyo::CheckMunyon(void)
{
	return 0 >= munyonCnt_;
}


void puyo::Draw(void)
{
	DrawOval(drawPos_.first.x, drawPos_.first.y, drawPos_.second.x, drawPos_.second.y, puyoCor_[id_], true);

	if (munyonCnt_ > 0)
	{
		munyonCnt_--;
	}

	if (mnyonFlg_.bit.left)
	{
		DrawBox(pos_.x, pos_.y - rad_.y, pos_.x - rad_.x, pos_.y + rad_.y , puyoCor_[id_], true);
	}
	if (mnyonFlg_.bit.right)
	{
		DrawBox(pos_.x, pos_.y - rad_.y, pos_.x + rad_.x, pos_.y + rad_.y , puyoCor_[id_], true);
	}
	if (mnyonFlg_.bit.up)
	{
		DrawBox(pos_.x - rad_.x, pos_.y, pos_.x + rad_.x, pos_.y - rad_.y , puyoCor_[id_], true);
	}
	if (mnyonFlg_.bit.down)
	{
		DrawBox(pos_.x - rad_.x, pos_.y, pos_.x + rad_.x, pos_.y + rad_.y, puyoCor_[id_], true);
	}
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

bool puyo::SetMnyonFlg(DirUnion flg)
{
	mnyonFlg_ = flg;
	return true;
}

void puyo::alive(bool flg)
{
	alive_ =  flg;
}

bool puyo::seiretu(void)
{
	return true;
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

const VecPair puyo::drawPos(void) const
{
	return drawPos_;
}

void puyo::Init(Vector2& pos,PuyoID id)
{
	puyoCor_.try_emplace(PuyoID::Red, 0xff0000);
	puyoCor_.try_emplace(PuyoID::Bule,0x0000ff);
	puyoCor_.try_emplace(PuyoID::Green,0x00ff00);
	puyoCor_.try_emplace(PuyoID::Yellow,0xffff00);
	puyoCor_.try_emplace(PuyoID::Purple,0xff00ff);
	puyoCor_.try_emplace(PuyoID::Ojama, 0xf);

	mnyonFlg_ = { 0,0,0,0 };
	pos_ = pos;
	id_ = id;
	softCntMax_ = 20;
	softCnt_ = 0;
	alive_ = true;
	speedUp_ = false;
	speed_ = size_ / 4;
	puyonCnt_ = 0;
	puyonOffset_ = 0;
}

void puyo::Init(int no)
{
}

void puyo::PuyonUpdate()
{
	auto puyonPos = abs(((puyonCnt_ + 6)) % (12) - (6));
	if (puyonCnt_ > 0)
	{
		puyonCnt_--;
	}
	drawPos_.first = { pos_.x ,pos_.y + puyonPos + (puyonOffset_ * puyonPos * 2) };
	drawPos_.second = { rad_.x + puyonPos, rad_.y - puyonPos };
}
