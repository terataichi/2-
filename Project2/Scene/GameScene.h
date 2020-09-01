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
	Scene scene()override;
	void SetMenuFlg(bool)override;

private:
	void Init(void);
	Scene scene_;

	int ojamaCnt_;
	std::vector<std::unique_ptr<Stage>> stage_;				// �X�e�[�W��z��ŊǗ����Đl���ύX���\�ɂ���
	bool oldMenuFlg_;
	bool menuFlg_;
};

