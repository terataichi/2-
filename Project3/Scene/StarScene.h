#pragma once
#include <memory>
#include <vector>
#include "TransitionScene.h"
#include "../common/Vector2.h"
#include "../Object/Star.h"


class StarScene :
    public TransitionScene
{
public:
	/// <summary>
	/// 引数に前のシーンと次のシーンを渡す
	/// </summary>
	/// <param name="prev"></param>
	/// <param name="next"></param>
	/// <returns></returns>
	StarScene(uniqueBase prev, uniqueBase next);

	~StarScene();

	uniqueBase Update(uniqueBase scene)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
private:

	std::vector<std::shared_ptr<Star>> starList_;		// 星管理用ベクター

	int colorCnt_;
	bool nextFlg_;
};

