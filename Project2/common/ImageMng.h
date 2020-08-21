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
		return *_sInstance;														// * ½Ï°ÄÎß²İÀ‚Ì’†g‚ğ•Ô‚·
	}

	VecInt& GetHandle(std::string name);										// ˆê–‡‰æ‘œ		
	VecInt& GetHandle(std::string name, Vector2 divSize, Vector2 size);			// •ªŠ„‰æ‘œ
private:
	struct ImageMngDeleter														// ²Ò°¼ŞÏÈ°¼Ş¬°‚Ì¶½ÀÑÃŞØ°À°
	{
		void operator()(ImageMng* imageMng)const
		{
			delete imageMng;
		}
	};

	ImageMng();
	~ImageMng();																// ²Ò°¼ŞÏÈ°¼Ş¬°‚ÌÃŞÌ«ÙÄÃŞØ°À°

	static std::unique_ptr<ImageMng, ImageMngDeleter>_sInstance;

	std::map<std::string, VecInt> imgMap_;										// ‰æ‘œID•Û‘¶—p
};

