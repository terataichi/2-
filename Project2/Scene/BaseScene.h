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
	virtual void Draw() = 0;								// �e�V�[����Draw
};

