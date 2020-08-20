#pragma once
#include <memory>
#include <map>
#include <string>
#include <vector>

#define lpImageMng ImageMng::GetInstance()

using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		return *_sInstance;	// * ½Ï°ÄÎß²İÀ‚Ì’†g‚ğ•Ô‚·
	}

	int GetHandle(std::string name);
private:
	struct ImageMngDeleter	// ²Ò°¼ŞÏÈ°¼Ş¬°‚Ì¶½ÀÑÃŞØ°À°
	{
		void operator()(ImageMng* imageMng)const
		{
			delete imageMng;
		}
	};

	ImageMng();
	~ImageMng();	// ²Ò°¼ŞÏÈ°¼Ş¬°‚ÌÃŞÌ«ÙÄÃŞØ°À°

	static std::unique_ptr<ImageMng, ImageMngDeleter>_sInstance;

	std::map<std::string, int> imgMap_;

};

