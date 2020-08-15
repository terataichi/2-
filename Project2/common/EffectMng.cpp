#include "EffectMng.h"
#include <EffekseerForDXLib.h>

std::unique_ptr<EffectMng, EffectMng::EffectMngDeleter>EffectMng::_sInstance(new EffectMng());

void EffectMng::Update(void)
{
	UpdateEffekseer2D();

	// リストの中身でエフェクトの再生が終わっているものの中身を消す
	auto itr = std::remove_if(playList_.begin(), playList_.end(), [](int handle) {return !IsEffekseer2DEffectPlaying(handle); });
	playList_.erase(itr, playList_.end());
}

void EffectMng::Draw(void)
{
	DrawEffekseer2D();
}

bool EffectMng::StopAllEffect(void)
{
	for (auto list : playList_)
	{
		StopEffekseer2DEffect(list);
	}
	return true;
}

bool EffectMng::PlayEffect(std::string name, const Vector2& pos)
{
	playList_.push_front(PlayEffekseer2DEffect(GetHandle(name)));
	GetEffekseer2DManager()->SetAllColor(effectMap_[name], Effekseer::Color{255,0,0});
	SetPosPlayingEffekseer2DEffect(*(playList_.begin()), static_cast<float>(pos.x), static_cast<float>(pos.y), 0);
	return true;
}

bool EffectMng::Init(Vector2&& size)
{
	if (Effekseer_Init(8000) == -1)
	{
		return false;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	Effekseer_Set2DSetting(size.x, size.y);

	SetUseZBuffer3D(TRUE);

	SetWriteZBuffer3D(TRUE);

	return true;
}

int EffectMng::GetHandle(std::string name)
{
	// + name + でくっつけることができる便利!
	effectMap_.try_emplace(name, LoadEffekseerEffect(("effect/" + name + ".efk").c_str(), 100.0f));
	return effectMap_[name];
}

EffectMng::EffectMng()
{
}

EffectMng::~EffectMng()
{
}
