#include "Button.h"
#include <DxLib.h>
#include "../Scene/SceneMng.h"
#include "ImageMng.h"
#include "ButtonMove/MoveLeftRight.h"
#include "ButtonMove/MoveNon.h"
#include "ButtonMove/MoveUpDown.h"
#include "ButtonMove/MoveScale.h"


Button::Button(std::string& name, Vector2&& pos, Vector2&& size,int rate, float angle)
{
	Init(name, pos, size, rate, angle);
}

Button::~Button()
{
}

void Button::Update()
{
	moveCnt_ = (movePattern_ != oldPattern_) ? 0 : moveCnt_++;				// 動きが変わるときにカウントをリセットする
	moveFunc[movePattern_](pos_, angle_,exRate_, moveCnt_);					// 動きに対応した関数オブジェクトの呼び出し

	Draw();
}

bool Button::CheckHitButton(Vector2 pos)
{
	return (pos_.x < pos.x) && (pos.x < pos_.x + size_.x) && (pos_.y < pos.y) && (pos.y < pos_.y + size_.y);
}

void Button::pos(Vector2 pos)
{
	pos_ = pos;
}

const Vector2& Button::pos() const
{
	return pos_;
}

const float& Button::angle(void) const
{
	return angle_;
}

void Button::angle(float angle)
{
	angle_ = angle;
}

void Button::movePattern(MovePattern pattern)
{
	movePattern_ = pattern;
}

void Button::exRate(int exRate)
{
	exRate_ = exRate;
}

bool Button::Init(std::string name, Vector2 pos, Vector2 size, int rate, float angle)
{
	pos_ = pos;
	stdPos_ = pos;
	angle_ = angle;
	stdAngle_ = angle;
	size_ = size;
	moveCnt_ = 0;
	movePattern_ = MovePattern::Non;
	oldPattern_ = movePattern_;
	id_ = name;
	exRate_ = rate;
	stdExRate_ = rate;

	// 動きのパターンに合わせた関数オブジェクトの登録
	moveFunc.try_emplace(MovePattern::Non, MoveNon());
	moveFunc.try_emplace(MovePattern::LeftRight, MoveLeftRight());
	moveFunc.try_emplace(MovePattern::UpDown, MoveUpDown());
	moveFunc.try_emplace(MovePattern::Scale, MoveScale());


	return true;
}

void Button::Draw()
{
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle(id_.c_str())[0],pos_.x,pos_.y,exRate_,angle_,-1 });
}
