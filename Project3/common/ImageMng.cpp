#include "ImageMng.h"
#include <DxLib.h>

std::unique_ptr<ImageMng, ImageMng::ImageMngDeleter> ImageMng::_sInstance(new ImageMng);

VecInt& ImageMng::GetHandle(std::string name)
{
	// + name + でくっつけることができる便利!
	if (imgMap_.find(name) == imgMap_.end())
	{
		imgMap_[name].resize(1);
		imgMap_[name][0] = LoadGraph(("Image/" + name + ".png").c_str());
	}
	return imgMap_[name];
}

VecInt& ImageMng::GetHandle(std::string name, Vector2 divSize, Vector2 size)
{

	// ファインドをするとBeginからendまで探してくれる
	if (imgMap_.find(name) == imgMap_.end())
	{
		imgMap_[name].resize(divSize.x * divSize.y);
		LoadDivGraph((name).c_str(), divSize.x * divSize.y, divSize.x, divSize.y, size.x, size.y, &imgMap_[name][0]);
	}

	return imgMap_[name];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}