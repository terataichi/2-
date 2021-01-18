#include "Star.h"
#include "../common/ImageMng.h"
#include <DxLib.h>

Star::Star(Vector2 pos, Vector2 speed, int num) :pos_(pos),speed_(speed)
{
	rad_ = 0;

	colorNum_ = num;

	if (colorNum_ >= COLOR_MAX || colorNum_ < 0)
	{
		colorNum_ = 0;
	}

	colorCode_.try_emplace(0, 0xffff00);
	colorCode_.try_emplace(1, 0xff69b4);
	colorCode_.try_emplace(2, 0x0000ff);
	colorCode_.try_emplace(3, 0xff0000);
	colorCode_.try_emplace(4, 0xff00ff);
	colorCode_.try_emplace(5, 0x00ff00);
}

Star::~Star()
{
}

void Star::Update()
{
	pos_ += speed_;
	rad_+= 0.03f;
}

void Star::Draw()
{
	auto handle = lpImageMng.GetHandle("Image/star.png", Vector2{ 1,COLOR_MAX }, Vector2{ 91,87 })[colorNum_];
	DrawRotaGraph(pos_.x, pos_.y, 2, rad_, handle, true);
}

const Vector2& Star::pos() const
{
	return pos_;
}

const int Star::colorNum() const
{
	return colorNum_;
}

const int Star::colorCode()
{
	return colorCode_[colorNum_];
}
