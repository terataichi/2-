#pragma once
#include <memory>
#include <vector>
#include "../Stage.h"
#include "../common/Vector2.h"
#include "../Scene/BaseScene.h"

#define lpSceneMng SceneMng::GetInstance()

enum class DrawQue
{
	Image,
	X,
	Y,
	EX,						// �g��{��
	Angle,
	ZOrder,
};

//						ImageID, ���W,		�{��	�p�x,	���s
using drawQueT = std::tuple<int, int, int, int, float, int>;

class SceneMng
{
public:
	static SceneMng& GetInstance(void)
	{
		Create();
		return *sInstance_;
	}
	static void Create()
	{
		if (sInstance_ == nullptr)
		{
			sInstance_ = new SceneMng();
		}
	}
	static void Destroy()									// �ĂіY��ɒ���
	{
		if (sInstance_ != nullptr)
		{
			delete sInstance_;
		}
		sInstance_ = nullptr;
	}

	void Run(void);
	bool AddDrawQue(drawQueT que);

	const int pyoSize_;										// �Ղ�̑傫��
	const int pyoRadius_;									// �Ղ�̔��a
	const Vector2 gameOffSet_;								// �I�t�Z�b�g
	const Vector2 gameSize_;								// �Q�[����ʂ݂̂̃T�C�Y
	const Vector2 _frameSize;								// �Q�[����ʂ̎���̃t���[���̃T�C�Y
	const Vector2 screenSize_;								// �X�N���[���T�C�Y
private:
	bool SysInit(void);
	void Init(void);
	void Draw(void);										// �`��

	uniqueBase activeScene_;								// �V�[���Ǘ�

	static SceneMng* sInstance_;

	std::vector<drawQueT>drawList_;		// �`�惊�X�g

	SceneMng();
	~SceneMng();
};

