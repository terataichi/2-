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
		return *_sInstance;	// * �ϰ��߲���̒��g��Ԃ�
	}

	int GetHandle(std::string name);
private:
	struct ImageMngDeleter	// �Ұ���Ȱ�ެ��̶�����ذ��
	{
		void operator()(ImageMng* imageMng)const
		{
			delete imageMng;
		}
	};

	ImageMng();
	~ImageMng();	// �Ұ���Ȱ�ެ�����̫����ذ��

	static std::unique_ptr<ImageMng, ImageMngDeleter>_sInstance;

	std::map<std::string, int> imgMap_;

};

