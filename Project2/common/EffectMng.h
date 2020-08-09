#pragma once
#include <list>
#include <map>
#include <memory>
#include <string>
#include "Vector2.h"

#define lpEffectMng EffectMng::GetInstance()

class EffectMng
{
public:
	static EffectMng& GetInstance(void)
	{
		return *_sInstance;												// * ｽﾏｰﾄﾎﾟｲﾝﾀの中身を返す
	}

	void Update(void);
	void Draw(void);													// 描画
	bool StopAllEffect(void);											// 全エフェクト停止
	bool PlayEffect(std::string, const Vector2& pos);					// エフェクトの
	bool Init(Vector2&& size);											// 引数は画面サイズ
	int GetHandle(std::string name);									// 新しくハンドルを追加（なかったら）

private:
	struct EffectMngDeleter												// エフェクトマネージャーのカスタムデリータ
	{
		void operator()(EffectMng* effectMng)const
		{
			delete effectMng;
		}
	};


	EffectMng();
	~EffectMng();

	static std::unique_ptr<EffectMng, EffectMngDeleter>_sInstance;

	std::map<std::string, int> effectMap_;
	std::list<int> playList_;
};
