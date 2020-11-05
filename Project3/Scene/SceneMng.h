#pragma once
#include <memory>
#include <vector>
#include "../common/Vector2.h"
#include "../Scene/LoginScene.h"

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
using drawQueT = std::tuple<int, int, int, double, float, int>;

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
	void SetEnd(void);

	const Vector2 screenSize_;								// �X�N���[���T�C�Y
private:
	bool SysInit(void);
	void Init(void);
	void Draw(void);										// �`��

	uniqueBase activeScene_;								// �V�[���Ǘ�

	static SceneMng* sInstance_;

	std::vector<drawQueT>drawList_;							// �`�惊�X�g

	bool end_;												// ���s���� true : ���s  false : ���s�I��

	SceneMng();
	~SceneMng();
};

