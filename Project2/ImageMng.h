#pragma once
#include <memory>
#include <map>
#include <string>
#include <vector>

using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		return *_sInstance;	// * ｽﾏｰﾄﾎﾟｲﾝﾀの中身を返す
	}

private:
	struct ImageMngDeleter	// ｲﾒｰｼﾞﾏﾈｰｼﾞｬｰのｶｽﾀﾑﾃﾞﾘｰﾀｰ
	{
		void operator()(ImageMng* imageMng)const
		{
			delete imageMng;
		}
	};

	ImageMng();
	~ImageMng();	// ｲﾒｰｼﾞﾏﾈｰｼﾞｬｰのﾃﾞﾌｫﾙﾄﾃﾞﾘｰﾀｰ

	static std::unique_ptr<ImageMng, ImageMngDeleter>_sInstance;

	std::map<std::string, VecInt> imgMap;

};

