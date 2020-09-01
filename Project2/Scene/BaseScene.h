#pragma once
#include <memory>

class BaseScene;

enum class Scene
{
	Base,
	Title,
	Game,
	GameOver,
	GameEnd,
	Menu,
};

using uniqueBase = std::unique_ptr<BaseScene>;

// �e�V�[�����ʂ̃����o
class BaseScene
{
public:
	BaseScene();											// �R���X�g���N�^
	virtual ~BaseScene();									// �p����̃f�X�g���N�^���Ă�
	virtual uniqueBase Update(uniqueBase scene) = 0;		// �����͍��̃V�[����n���A�V�[����ς���Ƃ��͕Ԃ�l��
	virtual void Draw() = 0;								// �e�V�[����Draw
	virtual Scene scene() = 0;								// ���������̃V�[���Ȃ̂�����
	virtual void SetMenuFlg(bool set);						// ���j���[���J����Ă��邩�ǂ������Z�b�g����

protected:
	bool menuFlg_;
	Scene scene_;
};

