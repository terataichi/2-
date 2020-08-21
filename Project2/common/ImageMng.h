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
		return *_sInstance;														// * �ϰ��߲���̒��g��Ԃ�
	}

	VecInt& GetHandle(std::string name);										// �ꖇ�摜		
	VecInt& GetHandle(std::string name, Vector2 divSize, Vector2 size);			// �����摜
private:
	struct ImageMngDeleter														// �Ұ���Ȱ�ެ��̶�����ذ��
	{
		void operator()(ImageMng* imageMng)const
		{
			delete imageMng;
		}
	};

	ImageMng();
	~ImageMng();																// �Ұ���Ȱ�ެ�����̫����ذ��

	static std::unique_ptr<ImageMng, ImageMngDeleter>_sInstance;

	std::map<std::string, VecInt> imgMap_;										// �摜ID�ۑ��p
};

