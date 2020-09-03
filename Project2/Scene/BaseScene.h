#pragma once
#include <memory>

class BaseScene;

enum class Scene
{
	Non,
	Base,
	Title,
	Game,
	GameOver,
	GameEnd,
	Menu,
};

enum class FadeMode
{
	FadeOut,
	Instance,
	Wait,
	FadeIn,
	Max,
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
	virtual bool nextUpdate(void);							// �{�^���������ꂽ��ɉ񂷃A�b�v�f�[�g
	void SetNextScene(Scene);
	Scene GetNextScene(void);

protected:

	bool menuFlg_;											// ���j���[���J����Ă��邩�ǂ���
	Scene scene_;											// �����̃V�[�����i�[
private:
	Scene nextScene_;										// ���̃V�[�������܂�����ݒ肷��
};

