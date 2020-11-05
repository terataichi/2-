#pragma once
#include <memory>

class BaseScene;

using uniqueBase = std::unique_ptr<BaseScene>;

// �e�V�[�����ʂ̃����o
class BaseScene
{
public:
	BaseScene();											// �R���X�g���N�^
	virtual ~BaseScene();									// �p����̃f�X�g���N�^���Ă�
	virtual uniqueBase Update(uniqueBase scene) = 0;		// �����͍��̃V�[����n���A�V�[����ς���Ƃ��͕Ԃ�l��
	virtual void Draw();									// �e�V�[����Draw
	virtual void DrawOwnScene() = 0;						// 							
	virtual void Init() = 0;								// ������

protected:
	int drawScreen_;										// �e�V�[���̕`��p
};

