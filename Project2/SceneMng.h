#pragma once
#include <memory>
#include <vector>
#include "Stage.h"
#include "common/Vector2.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance(void)
	{
		Create();
		return *_sInstance;
	}
	static void Create()
	{
		if (_sInstance == nullptr)
		{
			_sInstance = new SceneMng();
		}
	}
	static void Destroy()									// �ĂіY��ɒ���
	{
		if (_sInstance != nullptr)
		{
			delete _sInstance;
		}
		_sInstance = nullptr;
	}

	void Run(void);

	const int _pyoSize;										// �Ղ�̑傫��
	const int _pyoRadius;									// �Ղ�̔��a
	const Vector2 _gameOffSet;								// �I�t�Z�b�g
	const Vector2 _gameSize;								// �Q�[����ʂ݂̂̃T�C�Y
	const Vector2 _frameSize;								// �Q�[����ʂ̎���̃t���[���̃T�C�Y
	const Vector2 _screenSize;								// �X�N���[���T�C�Y
private:
	bool SysInit(void);
	void Init(void);
	void Draw(void);										// �`��

	std::vector<std::unique_ptr<Stage>> _stage;				// �X�e�[�W��z��ŊǗ����Đl���ύX���\�ɂ���
	static SceneMng* _sInstance;

	SceneMng();
	~SceneMng();
};

