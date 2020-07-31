#pragma once
#include <vector>
#include <map>
#include <memory>
#include <string>

using VecInt = std::vector<int>;

class EffectMng
{
public:
	static EffectMng& GetInstance(void)
	{
		return *_sInstance;	// * ｽﾏｰﾄﾎﾟｲﾝﾀの中身を返す
	}

private:
	struct EffectMngDeleter	// エフェクトマネージャーのカスタムデリータ
	{
		void operator()(EffectMng* imageMng)const
		{
			delete imageMng;
		}
	};

	EffectMng();
	~EffectMng();

	static std::unique_ptr<EffectMng, EffectMngDeleter>_sInstance;

	std::map<std::string, VecInt> imgMap;

};
