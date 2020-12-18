#pragma once
#include <vector>
#include "BaseScene.h"
#include "../NetWork/NetWork.h"

class ResultScene :
    public BaseScene
{
public:

	/// <summary>
	/// インスタンス時にリザルトのデータを渡す
	/// </summary>
	/// <param name="resultData"></param>
	ResultScene(std::vector<int>& resultData);

	~ResultScene();

	uniqueBase Update(uniqueBase scene)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
private:
	std::vector<int> resultData_;
};

