#include "Button.h"
#include <DxLib.h>
#include "../Scene/SceneMng.h"
#include "ImageMng.h"

Button::Button(std::string name, Vector2& pos, Vector2& size,int rate, float angle, int zlayer, int num)
{
	Init(name, pos, size, rate, angle,zlayer, num);
}

Button::~Button()
{
}

void Button::Update()
{
	moveCnt_ = (movePattern_ != oldPattern_) ?  0 : moveCnt_ += 1;		// 動きが変わるときにカウントをリセットする
	moveFunc[movePattern_]();											// 動きに対応した関数オブジェクトの呼び出し
}

bool Button::CheckHitButton(Vector2 pos)
{
	return (pos_.x <= pos.x) && (pos.x <= pos_.x + size_.x) && (pos_.y <= pos.y) && (pos.y <= pos_.y + size_.y);
}

void Button::pos(Vector2 pos)
{
	pos_ = pos;
}

const Vector2& Button::pos() const
{
	return pos_;
}

const Vector2& Button::stdPos(void) const
{
	return stdPos_;
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
	oldPattern_ = movePattern_;				// 前情報の格納
	movePattern_ = pattern;
}

void Button::exRate(int exRate)
{
	exRate_ = exRate;
}

bool Button::Init(std::string name, Vector2& pos, Vector2& size, int& rate, float angle, int zLayer, int num)
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
	zLayer_ = zLayer;
	buttonID_ = num;
	InitFunc();

	return true;
}

bool Button::InitFunc(void)
{
	// 動きのパターンに合わせた関数オブジェクトの登録
	moveFunc.try_emplace(MovePattern::Non, [&]() {});
	moveFunc.try_emplace(MovePattern::LeftRight,
		[&]()
		{
			pos_.x = static_cast<int>(stdPos_.x + 30 * cosf((moveCnt_ * 0.05f)));
		});

	moveFunc.try_emplace(MovePattern::UpDown,
		[&]()
		{
			pos_.y = static_cast<int>(stdPos_.y + 30 * sinf((moveCnt_ * 0.05f)));
		});
	moveFunc.try_emplace(MovePattern::Scale,
		[&]()
		{
			exRate_ = static_cast<double>(stdExRate_ + 0.1 * sin((static_cast<double>(moveCnt_) * 0.05)));
		});

	return true;
}

void Button::Draw()
{
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle(id_.c_str())[0],pos_.x,pos_.y,exRate_,angle_, zLayer_});
}

void Button::reset()
{
	pos_ = stdPos_;
	angle_ = stdAngle_;
	moveCnt_ = 0;
	exRate_ = stdExRate_;
}
