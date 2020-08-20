#include "ImageMng.h"
#include <DxLib.h>

std::unique_ptr<ImageMng, ImageMng::ImageMngDeleter> ImageMng::_sInstance(new ImageMng);

int ImageMng::GetHandle(std::string name)
{
	// + name + でくっつけることができる便利!
	imgMap_.try_emplace(name, LoadGraph(("image/" + name + ".png").c_str()));
	return imgMap_[name];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
