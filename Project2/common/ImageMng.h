#pragma once
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()

using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		return *_sInstance;														// * ｽﾏｰﾄﾎﾟｲﾝﾀの中身を返す
	}

	VecInt& GetHandle(std::string name);										// 一枚画像		
	VecInt& GetHandle(std::string name, Vector2 divSize, Vector2 size);			// 分割画像
private:
	struct ImageMngDeleter														// ｲﾒｰｼﾞﾏﾈｰｼﾞｬｰのｶｽﾀﾑﾃﾞﾘｰﾀｰ
	{
		void operator()(ImageMng* imageMng)const
		{
			delete imageMng;
		}
	};

	ImageMng();
	~ImageMng();																// ｲﾒｰｼﾞﾏﾈｰｼﾞｬｰのﾃﾞﾌｫﾙﾄﾃﾞﾘｰﾀｰ

	static std::unique_ptr<ImageMng, ImageMngDeleter>_sInstance;

	std::map<std::string, VecInt> imgMap_;										// 画像ID保存用
};

