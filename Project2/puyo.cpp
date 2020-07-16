#include <DxLib.h>
#include "puyo.h"
#include "SceneMng.h"

puyo::puyo() :size_(64), rad_(size_ / 2)
{
	Init();
}

puyo::~puyo()
{
}

void puyo::UpDate(void)
{
	if (lpSceneMng.gameSize_.y - rad_ < pos_.y)
	{
		pos_.y = lpSceneMng.gameSize_.y - rad_;
	}
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
		if (!dirFlg_.bit.up) { pos_.y -= size_; }
		break;
	case INPUT_ID::BUTTON_RIGHT:
		if (!dirFlg_.bit.right) { pos_.x += size_; }
		break;
	case INPUT_ID::BUTTON_DOWN:
		if (!dirFlg_.bit.down) { pos_.y += size_; }
		break;
	case INPUT_ID::BUTTON_ROTA:
		break;
	case INPUT_ID::MAX:
		break;
	default:
		break;
	}

}

void puyo::Draw(void)
{
	DrawCircle(pos_.x, pos_.y, rad_, 0xfffff, true);
}

bool puyo::SetDirFlg(DirUnion flg)
{
	dirFlg_ = flg;
	return true;
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
	return Vector2(pos_.x / size, pos_.y / size);
}

void puyo::Init(void)
{
	pos_ = Vector2(rad_ + size_ * 3, rad_);
	id_ = PuyoID::Green;
}
