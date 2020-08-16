#pragma once
#include <memory>
#include <vector>
#include "BaseScene.h"
#include "../Stage.h"

class GameScene :
    public BaseScene
{
public:
	GameScene();
	~GameScene();
	uniqueBase Update(uniqueBase own) override;
	void Draw(void)override;
private:
	void Init(void);

	int ojamaCnt_;
	std::vector<std::unique_ptr<Stage>> stage_;				// �X�e�[�W��z��ŊǗ����Đl���ύX���\�ɂ���

};

