#pragma once
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
	ResultScene(UnionVec& resultData);

	~ResultScene();

	uniqueBase Update(uniqueBase scene)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
private:
	UnionVec resultData_;
};

