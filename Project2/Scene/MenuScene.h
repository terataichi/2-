#pragma once
#include <vector>
#include "BaseScene.h"
#include "../common/Button.h"

class MenuScene :
	public BaseScene
{
public:
	MenuScene(uniqueBase& child, bool move_);
	~MenuScene();

	uniqueBase Update(uniqueBase own) override;
	void Draw(void)override;
private:
	void Init(void);

	bool move_;											// ���j���[�ŃA�b�v�f�[�g�𓮂������������Ȃ���
	uniqueBase childScene_;								// ���j���[���J�����Ƃ��ɑO�̃V�[����ۑ����Ă���
	std::vector<std::unique_ptr<Button>> button_;		// �ڂ���Ǘ��p
};

