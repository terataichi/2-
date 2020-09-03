#pragma once
#include <memory>
#include <vector>
#include "BaseScene.h"
#include "../Stage.h"

using StageVec = std::vector<std::unique_ptr<Stage>>;

class GameScene :
    public BaseScene
{
public:
	GameScene(StageVec& stage);
	~GameScene();
	uniqueBase Update(uniqueBase own) override;
	void Draw(void)override;
	Scene scene()override;
	void SetMenuFlg(bool)override;
private:
	void Init(void);
	void StageInit();
	Scene scene_;

	int ojamaCnt_;
	StageVec stage_;				// ステージを配列で管理して人数変更を可能にする
	bool oldMenuFlg_;
	bool menuFlg_;
};

