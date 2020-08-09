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
		return *_sInstance;												// * �ϰ��߲���̒��g��Ԃ�
	}

	void Update(void);
	void Draw(void);													// �`��
	bool StopAllEffect(void);											// �S�G�t�F�N�g��~
	bool PlayEffect(std::string, const Vector2& pos);					// �G�t�F�N�g��
	bool Init(Vector2&& size);											// �����͉�ʃT�C�Y
	int GetHandle(std::string name);									// �V�����n���h����ǉ��i�Ȃ�������j

private:
	struct EffectMngDeleter												// �G�t�F�N�g�}�l�[�W���[�̃J�X�^���f���[�^
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
